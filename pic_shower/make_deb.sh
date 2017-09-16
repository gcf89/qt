#!/bin/sh

set -e
set -u

# variables
debdir=deb
builddir=$1
src_bin_name=pic_shower
dst_bin_name=imganalizer
progver=1.0.0
# debver=$(date +%Y.%m.%d_%T)
debver=$(date +%s)

# create deb
mkdir -p "$debdir/usr/bin"
cp "$builddir/$src_bin_name" "$debdir/usr/bin/$dst_bin_name"
strip "$debdir/usr/bin/$dst_bin_name"

cd $debdir
hashdeep -c md5 -r usr/bin > DEBIAN/md5sums
chmod 644 DEBIAN/md5sums
cd ../

cp -f deb/DEBIAN/control deb/DEBIAN/control.bak
sed -i -e "s/__progver__/$progver/g" deb/DEBIAN/control
sed -i -e "s/__debver__/$debver/g" deb/DEBIAN/control

fakeroot dpkg-deb --build deb

debname="imganalizer-$progver-amd64.deb"
mv "deb.deb" $debname

lintian $debname

# restore
mv -f deb/DEBIAN/control.bak deb/DEBIAN/control
