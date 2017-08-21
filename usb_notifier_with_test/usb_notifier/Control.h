#pragma once

#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTextEdit>


// FOR RELEASE
#define ENABLE_ALL_LOCK

#ifdef ENABLE_ALL_LOCK
#define ENABLE_LOCK
#define SPLASH_STAYS_ON_TOP
#define DISABLE_CAD
#endif

// FOR DEBUG
//#define SHOW_TRAY_CONTEXT_MENU
//#define AUTO_UNLOCK_ENABLE
//#define SHOW_TRAY_DEBUG
//#define CONSOLE_DEBUG
//#define GUI_DEBUG
//#define WIN_HV_MOCK
#define DUMP_PARSE_DATA

// FEATURES
#define SILENT_TRAY


class Gen
{
public:
  int Width;
  int Height;
  int PosX;
  int PosY;

  QTextEdit* plbl;

public:

  void WriteToLbl(const QString& msg)
  {
    if (plbl)
      plbl->append(msg);
  }
  void SetLbl(QTextEdit* lbl)
  {
    plbl = lbl;
  }

public:
  static Gen& Instance()
  {
    static Gen theSingleInstance;
    return theSingleInstance;
  }
private:
  Gen()
  {
    plbl = nullptr;
  }
  Gen(const Gen& root);
  Gen& operator=(const Gen&);
};



inline void WriteDebug(QString msg)
{
#ifdef CONSOLE_DEBUG
  QString time = "["+QDateTime::currentDateTime().toString()+"] "+msg;
  qDebug() << time;
#ifdef GUI_DEBUG
  Gen::Instance().WriteToLbl(time);
#endif
#else
  Q_UNUSED(msg)
#endif
}
