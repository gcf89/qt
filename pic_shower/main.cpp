#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

#include "Logger.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  if (Logger::Init("log.txt")) {
    qInstallMessageHandler(Logger::MsgHandler);
  }
  qInfo() << "-= Session opened =-";

  MainWindow w;
  int ret = 0;
  if (w.ReadSettings() && w.Init() && w.CacheExistingFileNames()) {
    qInfo() << "Init OK";
    w.show();
    ret = a.exec();
  } else {
    ret = -1;
    qCritical() << "cannot init (" << ret << ")";
  }

  qInfo() << "-= Session closed =-";
  return ret;
}
