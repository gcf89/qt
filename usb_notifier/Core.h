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

  WidgetSplash*         mSplash;
  DialogInfo*           mInfo;
  QFileSystemWatcher*   mFileWatcher;
  qint64                mLastFileSize;
  QAction*              quitAction;
  QSystemTrayIcon*      trayIcon;
  QMenu*                trayIconMenu;
  QString               mSourcePath;
  QStringList           mGoodHardwareSNs;
  QStringList           mBadHardwareSNs;
#ifdef Q_OS_UNIX
  Display*              dpy;
#endif
  bool                  mIsLocked;
  QIcon                 mIconGreen;
  QIcon                 mIconRed;

public:
  bool Init(QString path);

private:
  void RunWatcher(const QString& path);
  bool Parse(QString data, qint64 filesize);
  void CreateTrayIcon();
  void ConsiderLock();
private slots:
  void UnlockSystem();
private:
  void LockSystem();
  void SetCADActionsEnabled(bool enabled);
  void SetTrayIconAsLocked(bool locked);
  void SetTrayMessage(QString message);

private slots:
  void onTargetFileChanged();

public:
  explicit Core(QObject *parent = 0);
  ~Core();
};
