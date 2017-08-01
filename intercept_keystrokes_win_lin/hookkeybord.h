#ifndef HOOKKEYBORD_H
#define HOOKKEYBORD_H

#include <QObject>
#include <QThread>

#ifdef Q_OS_WIN32 || Q_OS_WIN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define _WIN32_WINNT 0x0400
#include <Windows.h>

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif

class HookKeybord : public QObject
{
    Q_OBJECT
    public:
        static HookKeybord *instance() {
            if (m_instance == NULL)
                m_instance = new HookKeybord();

            return m_instance;
        }

        enum HookKey {
            Print = 0,
            Shift,
            Ctrl
        };

        void startHook();
        void endHook();

        bool isActive(){return m_start;}

     public slots:

        void press(int );

     private:
        HookKeybord(QObject *parent = 0);
        static HookKeybord *m_instance;

        #ifdef Q_OS_WIN32 || Q_OS_WIN
            HHOOK hhkLowLevelKybd;
        #endif

        bool m_start;

     signals:
        void keyPress(HookKeybord::HookKey );


};

#endif // HOOKKEYBORD_H
