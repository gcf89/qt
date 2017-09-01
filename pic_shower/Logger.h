#pragma once

#include <QMessageLogContext>
#include <QString>
#include <QFile>
#include <QTextStream>

class Logger
{
  static QFile          mLogFile;
  static QTextStream    mOutStream;

public:
  Logger() {}
  ~Logger();

  static bool Init(const QString& logPath);
//  static void MsgHandler(QtMsgType type, const char *msg);
  static void MsgHandler(QtMsgType type, const QMessageLogContext& logContext, const QString& msg);
};
