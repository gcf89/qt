#include <iostream>

#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QTextEdit>
#include <QScrollBar>

#include "Control.h"
#include "Core.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QApplication::setQuitOnLastWindowClosed(false);

  QString sourcePath;
  if (argc == 2) { // try read 'source path' from args
    sourcePath = QString(argv[1]);
  }

#ifdef GUI_DEBUG
  QTextEdit l;
  l.show();
  Gen::Instance().SetLbl(&l);
#endif

  WriteDebug("[" + QDateTime::currentDateTime().toString() + "] " + "Started");




  Core core;
  if (!core.Init(sourcePath)) {
    return -1;
  }

  return a.exec();
}
