#include <QApplication>

#include "Core.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QApplication::setQuitOnLastWindowClosed(false);

  QString sourcePath;
  if (argc == 2) { // try read 'source path' from args
    sourcePath = QString(argv[1]);
  }

  Core core;
  if (!core.Init(sourcePath)) {
    return -1;
  }

  return a.exec();
}
