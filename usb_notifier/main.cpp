#include <QApplication>
#include <QDebug>

#include "Widget.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QApplication::setQuitOnLastWindowClosed(false);

  QString sourcePath;
  if (argc == 2) { // try read 'source path' from args
    sourcePath = QString(argv[1]);
  }

  Widget w;
  if (!w.Load(sourcePath)) {
    qFatal("Cannot open target file!");
  }
  w.show();

  return a.exec();
}
