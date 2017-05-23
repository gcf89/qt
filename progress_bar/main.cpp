#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    w.runQProgressBar();
    w.runSingleProgressbar();
//    w.runDoubleProgressBar();

    return a.exec();
}
