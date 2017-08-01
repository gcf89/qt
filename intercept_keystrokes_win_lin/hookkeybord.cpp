#include "hookkeybord.h"
#include <QDebug>

#include <QtConcurrentRun>

#ifdef Q_OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <X11/Xlib.h>

void qt_x11_hookKey(HookKeybord *);
#endif

HookKeybord *HookKeybord::m_instance = NULL;

HookKeybord::HookKeybord(QObject *parent)
    :QObject(parent)
{
    m_start = false;
}

void HookKeybord::startHook()
{
    #ifdef Q_OS_WIN32 || Q_OS_WIN
      hhkLowLevelKybd =  SetWindowsHookEx(WH_KEYBOARD_LL, (LowLevelKeyboardProc), 0, 0);
    #endif

    #ifdef Q_OS_LINUX
      QtConcurrent::run(qt_x11_hookKey,this);
    #endif

    m_start = true;
}

void HookKeybord::endHook()
{
    #ifdef Q_OS_WIN32 || Q_OS_WIN
        UnhookWindowsHookEx(hhkLowLevelKybd);
    #endif

    m_start = false;
}

void HookKeybord::press(int key)
{
    qDebug() << Q_FUNC_INFO << key;
    emit keyPress(static_cast<HookKeybord::HookKey>(key));
}

#ifdef Q_OS_WIN32 || Q_OS_WIN
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

   BOOL fEatKeystroke = FALSE;

   if (nCode == HC_ACTION) {
      switch (wParam) {
      case WM_KEYDOWN:  case WM_SYSKEYDOWN:
      case WM_KEYUP:    case WM_SYSKEYUP:
         PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;

         fEatKeystroke = ((p->vkCode == VK_SNAPSHOT) && (((GetKeyState(VK_CONTROL) & 0x8000) != 0)) && (((GetKeyState(VK_SHIFT) & 0x8000) != 0)));
         if (fEatKeystroke)
         {
             HookKeybord::instance()->press(HookKeybord::Print | HookKeybord::Ctrl | HookKeybord::Shift);
             return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
         }


         fEatKeystroke = ((p->vkCode == VK_SNAPSHOT) && (((GetKeyState(VK_CONTROL) & 0x8000) != 0)));
         if (fEatKeystroke)
         {
             HookKeybord::instance()->press(HookKeybord::Print | HookKeybord::Ctrl);
             return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
         }

         fEatKeystroke = ((p->vkCode == VK_SNAPSHOT) && (((GetKeyState(VK_SHIFT) & 0x8000) != 0)));
         if (fEatKeystroke)
         {
             HookKeybord::instance()->press(HookKeybord::Print | HookKeybord::Shift);
             return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
         }

         fEatKeystroke=(p->vkCode == VK_SNAPSHOT);
         if (fEatKeystroke)
         {
             HookKeybord::instance()->press(HookKeybord::Print);
             return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
         }

         break;
      }
   }
   return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}
#endif

#ifdef Q_OS_LINUX
void qt_x11_hookKey(HookKeybord *hk)
{
    Display *dpy;
    XEvent ev;
    char *s;
    unsigned int kc;

    if (NULL==(dpy=XOpenDisplay(NULL))) {
       qDebug() << Q_FUNC_INFO << "error XOpenDisplay";
    }

    XGrabKeyboard(dpy, DefaultRootWindow(dpy),
                  true, GrabModeAsync, GrabModeAsync, CurrentTime);

    bool ctrlmodified = false;
    bool shiftmodified = false;

    while(hk->isActive()) {
       XNextEvent(dpy, &ev);
       switch (ev.type) {
          case KeyPress:
             kc = ((XKeyPressedEvent*)&ev)->keycode;
             s = XKeysymToString(XKeycodeToKeysym(dpy, kc, 0));
             qDebug() << Q_FUNC_INFO << s;
             if (!strcmp(s, "Control_L")) ctrlmodified=true;
             if (!strcmp(s, "Control_R")) ctrlmodified=true;
             if (!strcmp(s, "Shift_L")) shiftmodified=true;
             if (!strcmp(s, "Shift_R")) shiftmodified=true;

             if (!strcmp(s, "Print") && ctrlmodified && shiftmodified) {
                hk->press(HookKeybord::Print | HookKeybord::Shift | HookKeybord::Ctrl);
                continue;
             }

             if (!strcmp(s, "Print") && ctrlmodified) {
                hk->press(HookKeybord::Print | HookKeybord::Ctrl);
                continue;
             }

             if (!strcmp(s, "Print") && shiftmodified) {
                hk->press(HookKeybord::Print | HookKeybord::Shift);
                continue;
             }

             if (!strcmp(s, "Print")) {
                hk->press(HookKeybord::Print);
                continue;
             }

             XSendEvent(dpy, 0, false, KeyPressMask,&ev);
             break;
          case Expose:
                while (XCheckTypedEvent(dpy, Expose, &ev)) /* empty body */ ;
             break;
          case ButtonPress:
          case ButtonRelease:
          case KeyRelease:
             kc = ((XKeyReleasedEvent*)&ev)->keycode;
             s = XKeysymToString(XKeycodeToKeysym(dpy, kc, 0));


             if (!strcmp(s, "Control_L")) ctrlmodified=false;
             if (!strcmp(s, "Control_R")) ctrlmodified=false;
             if (!strcmp(s, "Shift_L")) shiftmodified=false;
             if (!strcmp(s, "Shift_R")) shiftmodified=false;
          case MotionNotify:
          case ConfigureNotify:
          default:
             break;
       }
    }

    XUngrabKeyboard(dpy, CurrentTime);

    if (XCloseDisplay(dpy)) {
       qDebug() << Q_FUNC_INFO << "error XCloseDisplay";
    }
}
#endif
