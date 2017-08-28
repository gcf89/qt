#include "MainWindow.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  MainWindow w;
  int ret = 0;
  if (w.ReadSettings() && w.CacheExistingFileNames() && w.Init()) {
    w.show();
    ret = a.exec();
  } else {
    qDebug() << "ERR: cannot init properly";
    ret = -1;
  }

  return ret;
}
