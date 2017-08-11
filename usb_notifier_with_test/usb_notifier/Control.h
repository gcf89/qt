#pragma once

#include <QString>
#include <QDebug>
#include <QDateTime>


// FOR RELEASE
#define ENABLE_LOCK
#define SPLASH_STAYS_ON_TOP
//#define DISABLE_CAD

// FOR DEBUG
//#define SHOW_TRAY_CONTEXT_MENU
//#define AUTO_UNLOCK_ENABLE
//#define SHOW_TRAY_DEBUG
#define SHOW_DEBUG

// FEATURES
#define SILENT_TRAY


class Gen
{
public:
  int Width;
  int Height;
  int PosX;
  int PosY;

public:
  static Gen& Instance()
  {
    static Gen theSingleInstance;
    return theSingleInstance;
  }
private:
  Gen(){}
  Gen(const Gen& root);
  Gen& operator=(const Gen&);
};


inline void WriteDebug(QString msg)
{
#ifdef SHOW_DEBUG
  qDebug()  << "[" << QDateTime::currentDateTime().toString() << "]" << msg;
#else
  Q_UNUSED(msg)
#endif
}
