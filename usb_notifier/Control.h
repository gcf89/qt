#pragma once

// FOR RELEASE
#define ENABLE_LOCK
#define SPLASH_STAYS_ON_TOP
#define DISABLE_CAD

// FOR DEBUG
//#define SHOW_TRAY_CONTEXT_MENU
#define AUTO_UNLOCK_ENABLE
#define SHOW_TRAY_DEBUG
#define SHOW_DEBUG

// FEATURES
//#define SILENT_TRAY




#include <QDebug>
#include <QString>

inline void WriteDebug(QString msg)
{
#ifdef SHOW_DEBUG
  qDebug() << msg;
#else
  Q_UNUSED(msg)
#endif
}
