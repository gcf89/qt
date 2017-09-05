#include <iostream>

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QDebug>

#include "Logger.h"


const QString kFormat = "[yyyy/MM/dd hh:mm:ss.zzz]";


QFile Logger::mLogFile;
QTextStream Logger::mOutStream;


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
    mOutStream.setDevice(&mLogFile);
    mOutStream << QDateTime::currentDateTime().toString(kFormat)
               << " Info : Logger initialization OK\n";
    std::cout << QDateTime::currentDateTime().toString(kFormat).toStdString()
              << " Info : Logger initialization OK"
              << std::endl;
  } else {
    std::cout << QDateTime::currentDateTime().toString(kFormat).toStdString()
              << " Error: Logger initialization FAILED"
              << std::endl;
  }

  return res;
}

void Logger::MsgHandler(QtMsgType type, const QMessageLogContext &logContext, const QString &msg)
{
  Q_UNUSED(logContext)

  QDebug dbg(type);

  switch (type){
  case QtInfoMsg:
    mOutStream << QDateTime::currentDateTime().toString(kFormat) <<" Info : " << msg <<  "\n";
    dbg << QDateTime::currentDateTime().toString(kFormat) << "Info :" << msg;
    break;
  case QtDebugMsg:
#ifndef QT_NO_DEBUG
    mOutStream << QDateTime::currentDateTime().toString(kFormat) <<" Debug: " << msg <<  "\n";
    dbg << QDateTime::currentDateTime().toString(kFormat) << "Debug:" << msg;
#endif
    break;
  case QtWarningMsg:
    mOutStream << QDateTime::currentDateTime().toString(kFormat) <<" Warn : " << msg <<  "\n";
    dbg << QDateTime::currentDateTime().toString(kFormat) << "Warn :" << msg;
    break;
  case QtCriticalMsg:
    mOutStream << QDateTime::currentDateTime().toString(kFormat) <<" Error: " << msg <<  "\n";
    dbg << QDateTime::currentDateTime().toString(kFormat) << "Error:" << msg;
    break;
  case QtFatalMsg:
    mOutStream << QDateTime::currentDateTime().toString(kFormat) <<" Fatal: " << msg <<  "\n";
    dbg << QDateTime::currentDateTime().toString(kFormat) << "Fatal:" << msg;
    break;
  }
  mOutStream.flush();
}
