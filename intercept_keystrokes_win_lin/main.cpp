#include <QtGui/QApplication>

#include "hookkeybord.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HookKeybord::instance()->startHook();

    return a.exec();
}
