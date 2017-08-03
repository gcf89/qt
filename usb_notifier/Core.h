#pragma once

#include <QObject>

#include <QWidget>
#include <QString>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>

#ifdef Q_OS_UNIX
#include <X11/Xlib.h>
#endif

#include "WidgetSplash.h"
#include "DialogInfo.h"


class Core : public QObject
{
  Q_OBJECT

  WidgetSplash* mSplash;
  DialogInfo* mInfo;

  QFileSystemWatcher*   mFileWatcher;
  qint64                mLastFileSize;

  QAction *quitAction;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  QString               mSourcePath;

  QStringList mGoodHardwareSNs;
  QStringList mBadHardwareSNs;

#ifdef Q_OS_UNIX
  Display *dpy;
#endif

  bool mIsLocked;

  QIcon mIconGreen;
  QIcon mIconRed;

public:
  bool Init(QString path);

private:
  void RunWatcher(const QString& path);
  bool Parse(QString data, qint64 filesize);
  void CreateActions();
  void CreateTrayIcon();
  void ConsiderLock();
  void UnlockSystem();
  void LockSystem();
  void SetTrayIconAsLocked(bool locked);

private slots:
  void onTargetFileChanged();

public:
  explicit Core(QObject *parent = 0);
  ~Core();
};
