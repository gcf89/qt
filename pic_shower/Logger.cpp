#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QDebug>

#include "Logger.h"


Logger::~Logger()
{
  if (mLogFile.isOpen()) {
    mLogFile.close();
  }
}

bool Logger::Init(const QString& logPath)
{
  mLogFile.setFileName(logPath);
  bool res = mLogFile.open(QIODevice::Append | QIODevice::Text);
  if (res) {
    qDebug() << "OK: Logger initialization";
  } else {
    qDebug() << "ERR: Logger initialization";
  }

  return res;
}

void Logger::MsgHandler(QtMsgType type, const QMessageLogContext &logContext, const QString &msg)
{
  QTextStream out(&logFile);
  QDebug dbg;
  switch (type){
  case QtDebugMsg:
    out << QDateTime::currentDateTime().toString() <<" Debug: " << msg <<  "\n";
    dbg. << QDateTime::currentDateTime().toString() <<" Debug: " << msg <<  "\n";
    break;
  case QtWarningMsg:
    out << QDateTime::currentDateTime().toString() <<" Warning: " << msg <<  "\n";
    dbg << QDateTime::currentDateTime().toString() <<" Warning: " << msg <<  "\n";
    break;
  case QtCriticalMsg:
    out << QDateTime::currentDateTime().toString() <<" Critical: " << msg <<  "\n";
    dbg << QDateTime::currentDateTime().toString() <<" Critical: " << msg <<  "\n";
    break;
  case QtFatalMsg:
    out << QDateTime::currentDateTime().toString() <<" Fatal: " << msg <<  "\n";
    dbg << QDateTime::currentDateTime().toString() <<" Fatal: " << msg <<  "\n";
    break;
  }
}

//void Logger::MsgHandler(QtMsgType type, const char *msg)
//{
//  QTextStream out(&logFile);
//  QDebug dbg;
//  switch (type){
//  case QtDebugMsg:
//    out << QDateTime::currentDateTime().toString() <<" Debug: " << msg <<  "\n";
//    dbg. << QDateTime::currentDateTime().toString() <<" Debug: " << msg <<  "\n";
//    break;
//  case QtWarningMsg:
//    out << QDateTime::currentDateTime().toString() <<" Warning: " << msg <<  "\n";
//    dbg << QDateTime::currentDateTime().toString() <<" Warning: " << msg <<  "\n";
//    break;
//  case QtCriticalMsg:
//    out << QDateTime::currentDateTime().toString() <<" Critical: " << msg <<  "\n";
//    dbg << QDateTime::currentDateTime().toString() <<" Critical: " << msg <<  "\n";
//    break;
//  case QtFatalMsg:
//    out << QDateTime::currentDateTime().toString() <<" Fatal: " << msg <<  "\n";
//    dbg << QDateTime::currentDateTime().toString() <<" Fatal: " << msg <<  "\n";
//    break;
//  }
//}
