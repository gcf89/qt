#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>
#include <QTimer>
#include <QThread>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QColor>
#include <QPixmap>
#include <QIcon>
#include <QSettings>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include "Core.h"
#include "Control.h"


#ifdef Q_OS_WIN32
UINT fuiPreviousState; // wtf?

HHOOK hKeyHook = NULL;
HHOOK hMouseHook = NULL;

LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  Q_UNUSED(nCode)
  Q_UNUSED(wParam)
  Q_UNUSED(lParam)
  return -1;
}
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  Q_UNUSED(nCode)
  Q_UNUSED(wParam)
  Q_UNUSED(lParam)
  return -1;
}

BOOL LockSpecialKeys(PUINT fuiState)
{
  return SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, TRUE, fuiState, 0);
}

BOOL UnlockSpecialKeys(PUINT fuiState)
{
  return SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, FALSE, fuiState, 0);
}
#endif

const int kHideTimeout = 1500;
const int kTrayHideTimeout = 2000;


bool Core::Init(QString cmdPath)
{
  QFile f;
  if (cmdPath.isEmpty()) {
    QString confPath = qApp->applicationDirPath()+"/config";

    f.setFileName(confPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
      SetTrayMessage("ERROR: cannot open config file: " + confPath);
      return false;
    }
    QString targetPath = f.readAll();
    f.close();

    if (targetPath.startsWith("./")) {
      targetPath.remove(0, 1);
      targetPath.prepend(qApp->applicationDirPath());
    }

    f.setFileName(targetPath);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
      RunWatcher(targetPath);
    } else {
      SetTrayMessage("ERROR: cannot open file: " + targetPath);
      return false;
    }
  } else {
    if (cmdPath.startsWith("./")) {
      cmdPath.remove(0, 1);
      cmdPath.prepend(qApp->applicationDirPath());
    }

    f.setFileName(cmdPath);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
      RunWatcher(cmdPath);
    } else {
      SetTrayMessage("ERROR: cannot open file:" + cmdPath);
      return false;
    }
  }

  return true;
}

void Core::RunWatcher(const QString& path)
{
  mSourcePath = path;

//  QFile f(mSourcePath);
//  mLastFileSize = f.size();

  // 1. on start read all file, parse and consider lock
  // 2. react on specific messages (lock or unlock)
  // 3. get real hardware and test

  mFileWatcher->addPath(mSourcePath);
  connect(mFileWatcher, SIGNAL(fileChanged(QString)),
          this, SLOT(onTargetFileChanged()));

  onTargetFileChanged();
}

bool Core::Parse(QString data, qint64 filesize)
{
  Q_UNUSED(filesize)

  QString id;
  QString serialNumber;
  QString text;
  QString timestamp;

  data = data.simplified();

  int fromPos = 0;
  int toPos = 0;


  QStringList lines = data.split("},{");
  WriteDebug("Found lines: "+QString::number(lines.size()));

  int err;
  for (auto it=lines.constBegin(); it!=lines.constEnd(); ++it) {
    QString line = *it;

    err = 0;
    if ( (fromPos = line.indexOf( R"("id")")) != -1 ) {
      ++err;
      if ( (toPos = line.indexOf(',', fromPos)) != -1 ) {
        ++err;
        id = line.mid(fromPos, toPos - fromPos);

        if ( (fromPos = line.indexOf( R"("serial_number")")) != -1 ) {
          ++err;
          if ( (toPos = line.indexOf( R"("})", fromPos) ) != -1 ) {
            ++err;
            serialNumber = line.mid(fromPos, toPos - fromPos);

            if ( (fromPos = line.indexOf( R"("text")")) != -1 ) {
              ++err;
              if ( (toPos = line.indexOf( R"(",)", fromPos)) != -1 ) {
                ++err;
                text = line.mid(fromPos, toPos - fromPos);

                if ( (fromPos = line.indexOf( R"("timestamp")")) != -1 ) {
                  ++err;
                  if ( (toPos = line.indexOf(R"(")", fromPos)) != -1 ) {
                    ++err;
                    fromPos +=1;
                    timestamp = line.mid(fromPos, toPos - fromPos);

                    QString sn = serialNumber.section(":",1,1).replace(0,1,"");
                    QString msg = text.section(":",1,1).replace(0,1,"");
                    QString tsp = timestamp.section(":",1,1).replace(0,1,"");

#ifdef SHOW_DEBUG
                    qDebug() << "\nNew device found:\n"
                             << id
                             << serialNumber << "(" << sn << ")"
                             << text << "(" << msg << ")"
                             << timestamp << "(" << tsp << ")";
#endif

                    if ( msg == QString::fromUtf8("USB устройство разрешено") ) {
//                      if (!mHWInstalledRemoved.removeOne(sn)) {
//                        mHWGood.removeOne(sn);
//                      }

                      if (mHWInstalledRemoved.removeOne(sn)) {
                        WriteDebug("we got back needed hw! yay!");
                      } else {
                        mHWGood << sn;
                        WriteDebug("inserted smth good, but not needed");
                      }

                    } else if ( msg == QString::fromUtf8("USB устройство заблокировано") ) {
                      mHWBad << sn;
                      WriteDebug("inserted smth bad");

                    } else if ( msg == QString::fromUtf8("USB устройство извлечено") ) {
//                      if (!mHWBad.removeOne(sn)) {
//                        mHWGood << sn;
//                      }

                      if (mHWBad.removeOne(sn)) {

                        WriteDebug("bad removed, cool!");

                      } else if (mHWGood.removeOne(sn)) {

                        WriteDebug("good removed, ok, nothing special here");

                      } else {

                        mHWInstalledRemoved << sn; //
                        WriteDebug("removed smth needed, lock needed!");

                      }


                    } else {
                      qDebug() << "PASS:" << msg;
                    }

                  }
                }
              }
            }
          }
        }
      }
    }

    switch (err) {
    case 0:
      SetTrayMessage("Failed to find ID start!");
      break;
    case 1:
      SetTrayMessage("Failed to find ID end!");
      break;
    case 2:
      SetTrayMessage("Failed to find SERIAL start!");
      break;
    case 3:
      SetTrayMessage("Failed to find SERIAL end!");
      break;
    case 4:
      SetTrayMessage("Failed to find TEXT start!");
      break;
    case 5:
      SetTrayMessage("Failed to find TEXT end!");
      break;
    case 6:
      SetTrayMessage("Failed to find TIMESTAMP start!");
      break;
    case 7:
      SetTrayMessage("Failed to find TIMESTAMP end!");
      break;
    }

    if (err != 8) { // not all tags are found
      auto next = it; // do not save (return false) file size (assume new data will arrive soon)
      if (++next == lines.constEnd())  {
        return false;
      }
    }
  }

  // comment if testing
//  ConsiderLock();
  return true;



//  while ( (fromPos = data.indexOf("\"id\"")) != -1 ) {
//    if ( (toPos = data.indexOf(",", fromPos)) != -1 ) {
//      id = data.mid(fromPos, toPos - fromPos);

//      if ( (fromPos = data.lastIndexOf("\"serial_number\"")) != -1 ) {
//        if ( (toPos = data.indexOf("\"}", fromPos)) != -1 ) {
//          serialNumber = data.mid(fromPos, toPos - fromPos);

//          if ( (fromPos = data.lastIndexOf("\"text\"")) != -1 ) {
//            if ( (toPos = data.indexOf("\",", fromPos)) != -1 ) {
//              text = data.mid(fromPos, toPos - fromPos);

//              if ( (fromPos = data.lastIndexOf("\"timestamp\"")) != -1 ) {
//                if ( (toPos = data.indexOf("\"}", fromPos)) != -1 ) {
//                  timestamp = data.mid(fromPos, toPos - fromPos);



//                  QString sn = serialNumber.section(":",1,1).replace(0,1,"");
//                  QString msg = text.section(":",1,1).replace(0,1,"");

//#ifdef SHOW_DEBUG
//                  qDebug() << "\n\nNew device found:\n"
//                           << id << sn << msg << timestamp;
//#endif

//                  if ( msg == QString::fromUtf8("USB устройство разрешено") ) {
//                    mGoodHardwareSNs.removeOne(sn);
//                  } else if ( msg == QString::fromUtf8("USB устройство заблокировано") ) {
//                    mBadHardwareSNs << sn;
//                  } else if ( msg == QString::fromUtf8("USB устройство извлечено") ) {
//                    if (!mBadHardwareSNs.removeOne(sn)) {
//                      mGoodHardwareSNs << sn;
//                    }
//                  } else {

//                  }

//                  ConsiderLock();

//                  return true;

//                } else {
//                  SetTrayMessage("Failed to find TIMESTAMP end!");

//                  // bad format, return false
//                }
//              } else {
//                SetTrayMessage("Failed to find TIMESTAMP start!");

//                // new cycle
//              }
//            } else {
//              SetTrayMessage("Failed to find TEXT end!");

//              // bad format, return false
//            }
//          } else {
//            SetTrayMessage("Failed to find TEXT start!");

//            // new cycle
//          }
//        } else {
//          SetTrayMessage("Failed to find SERIAL end!");

//          // format error, that's bad
//        }
//      } else {
//        SetTrayMessage("Failed to find SERIAL start!");

//        // try again to find 'id' tag i.e. 'continue'

//        // maybe incomplete or maybe there is no serial at all
//      }
//    } else {
//      SetTrayMessage("Failed to find ID end!");
//      // maybe incomplete data, wait for new
//      return false;
//    }
//  }

//  ConsiderLock();
//  return true;

//  if ( (fromPos = data.indexOf("\"id\"")) != -1 ) {
//    if ( (toPos = data.indexOf(",", fromPos)) != -1 ) {
//      id = data.mid(fromPos, toPos - fromPos);

//      if ( (fromPos = data.lastIndexOf("\"serial_number\"")) != -1 ) {
//        if ( (toPos = data.indexOf("\"}", fromPos)) != -1 ) {
//          serialNumber = data.mid(fromPos, toPos - fromPos);

//          if ( (fromPos = data.lastIndexOf("\"text\"")) != -1 ) {
//            if ( (toPos = data.indexOf("\",", fromPos)) != -1 ) {
//              text = data.mid(fromPos, toPos - fromPos);

//              if ( (fromPos = data.lastIndexOf("\"timestamp\"")) != -1 ) {
//                if ( (toPos = data.indexOf("\"}", fromPos)) != -1 ) {
//                  timestamp = data.mid(fromPos, toPos - fromPos);



//                  QString sn = serialNumber.section(":",1,1).replace(0,1,"");
//                  QString msg = text.section(":",1,1).replace(0,1,"");

//#ifdef SHOW_DEBUG
//                  qDebug() << "\n\nNew device found:\n"
//                           << id << sn << msg << timestamp;
//#endif

//// USB устройство извлечено
//                  // USB устройство извлечено
//                  // USB устройство заблокировано
////                  if ( msg == QString::fromUtf8("USB устройство разрешено") ) {
////                    mGoodHardwareSNs << sn;
////                  } else if ( msg == "gi" ) {
////                    mGoodHardwareSNs.removeOne(sn);
////                  } else if ( msg == "bi" ) {
////                    mBadHardwareSNs << sn;
////                  } else if ( msg == "br" ) {
////                    mBadHardwareSNs.removeOne(sn);
////                  }

//                  if ( msg == QString::fromUtf8("USB устройство разрешено") ) {
//                    mGoodHardwareSNs.removeOne(sn);
//                  } else if ( msg == QString::fromUtf8("USB устройство заблокировано") ) {
//                    mBadHardwareSNs << sn;
//                  } else if ( msg == QString::fromUtf8("USB устройство извлечено") ) {
//                    if (!mBadHardwareSNs.removeOne(sn)) {
//                      mGoodHardwareSNs << sn;
//                    }
//                  } else {

//                  }

//                  ConsiderLock();

//                  return true;

//                } else {
//                  SetTrayMessage("Failed to find TIMESTAMP end!");

//                  // bad format, return false
//                }
//              } else {
//                SetTrayMessage("Failed to find TIMESTAMP start!");

//                // new cycle
//              }
//            } else {
//              SetTrayMessage("Failed to find TEXT end!");

//              // bad format, return false
//            }
//          } else {
//            SetTrayMessage("Failed to find TEXT start!");

//            // new cycle
//          }
//        } else {
//          SetTrayMessage("Failed to find SERIAL end!");

//          // format error, that's bad
//        }
//      } else {
//        SetTrayMessage("Failed to find SERIAL start!");

//        // try again to find 'id' tag i.e. 'continue'
//      }
//    } else {
//      SetTrayMessage("Failed to find ID end!");
//    }
//  } else {
//    SetTrayMessage("Failed to find ID start!");

//    // return cause no id in all or next data found
//  }
//  return false;
}

void Core::CreateTrayIcon()
{
  trayIcon = new QSystemTrayIcon(mSplash);
  SetTrayIconAsLocked(false);

#ifdef SHOW_TRAY_CONTEXT_MENU
  quitAction = new QAction(tr("Выход"), this);
  connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

  trayIconMenu = new QMenu(mSplash);
  trayIconMenu->addAction(quitAction);

  trayIcon->setContextMenu(trayIconMenu);
#endif

  trayIcon->show();
}

void Core::ConsiderLock()
{
  bool needLock = !mHWGood.isEmpty() || !mHWBad.isEmpty();

  QString msg;
  if (needLock) {
    if (!mHWGood.isEmpty()) {
      msg.append(tr("Список извлеченного основного оборудования:"));
    }
    for (auto it=mHWGood.constBegin(); it!=mHWGood.constEnd(); ++it) {
      msg.append("\n").append(*it);
    }

    // separator
    if (!mHWGood.isEmpty()) {
      msg.append("\n\n");
    }

    if (!mHWBad.isEmpty()) {
      msg.append(tr("Список вставленных запрещенных устройств:"));
    }
    for (auto it=mHWBad.constBegin(); it!=mHWBad.constEnd(); ++it) {
      msg.append("\n").append(*it);
    }
  }

  mInfo->ShowInfo(msg, needLock);

  needLock ? LockSystem() : UnlockSystem();
}

void Core::onTargetFileChanged()
{
  QFile f(mSourcePath);
  if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qCritical("Error: cannot open target file!");
    return;
  }
  QTextStream in(&f);
  in.setCodec(QTextCodec::codecForName("UTF8"));


  if (mLastFileSize == f.size()) {
    SetTrayMessage("Current size: "
                   + QString::number(f.size())
                   + "Last size: "
                   + QString::number(mLastFileSize));

  } else if (mLastFileSize < f.size()) {

    if (in.seek(mLastFileSize)) {
      SetTrayMessage("Check for new data: "
                     + QString::number(mLastFileSize)
                     + ", "
                     + QString::number(f.size()));

      if (Parse(in.read(f.size() - mLastFileSize), f.size())) {
        mLastFileSize = f.size();

        SetTrayMessage("Parse OK: "
                       + QString::number(mLastFileSize)
                       + ", "
                       + QString::number(f.size()));
      } else {
        SetTrayMessage("Parse FAILED: "
                       + QString::number(mLastFileSize)
                       + ", "
                       + QString::number(f.size()));
      }
    } else {
      SetTrayMessage("Error: 'seek' failed");
    }
  } else {
    SetTrayMessage("File size is smaller than expected: "
                   + QString::number(mLastFileSize)
                   + ", "
                   + QString::number(f.size()));

    if (f.size() != 0) {
      mLastFileSize = 0;
      if (Parse(in.readAll(), f.size())) {
        mLastFileSize = f.size();
      }
    }

  }
}

void Core::UnlockSystem()
{
  if (mIsLocked) {
    mIsLocked = false;

#ifdef Q_OS_WIN32
#ifdef DISABLE_CAD
    SetCADActionsEnabled(true);
#endif
#endif

#ifdef ENABLE_LOCK
#ifdef Q_OS_WIN32
    UnlockSpecialKeys(&fuiPreviousState);

    UnhookWindowsHookEx(hKeyHook);
    UnhookWindowsHookEx(hMouseHook);
#endif

#ifdef Q_OS_UNIX
    XUngrabKeyboard(dpy, CurrentTime);
    XUngrabPointer(dpy, CurrentTime);

    if (XCloseDisplay(dpy)) {
      SetTrayMessage("error XCloseDisplay");
    }
#endif
#endif

#ifndef SILENT_TRAY
    trayIcon->showMessage(QString::fromUtf8("Внимание!"),
                          QString::fromUtf8("Система разблокирована!"),
                          QSystemTrayIcon::Information,
                          kTrayHideTimeout);
#endif

    SetTrayIconAsLocked(false);

    QTimer::singleShot(kHideTimeout, mSplash, SLOT(hide()));
    QTimer::singleShot(kHideTimeout, mInfo, SLOT(hide()));
  }
}

void Core::LockSystem()
{
  if (!mIsLocked) {
    mIsLocked = true;

#ifdef Q_OS_WIN32
#ifdef DISABLE_CAD
    SetCADActionsEnabled(false);
#endif
#endif

#ifdef ENABLE_LOCK
#ifdef Q_OS_WIN32
    LockSpecialKeys(&fuiPreviousState);

    hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (KeyProc), 0, 0);
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (MouseProc), NULL, 0);
#endif

#ifdef Q_OS_UNIX
    if (NULL==(dpy=XOpenDisplay(NULL))) {
      SetTrayMessage("error XOpenDisplay");
    }

    XGrabKeyboard(dpy, DefaultRootWindow(dpy), true, GrabModeAsync, GrabModeAsync, CurrentTime);

    XGrabPointer(dpy, DefaultRootWindow(dpy), 0, 0, 0, 0, DefaultRootWindow(dpy), None, CurrentTime);
#endif
#endif

    mSplash->show();
    mSplash->activateWindow();

    mInfo->show();
    mInfo->activateWindow();

    SetTrayIconAsLocked(true);

#ifndef SILENT_TRAY
    trayIcon->showMessage(QString::fromUtf8("Внимание!"),
                          QString::fromUtf8("Система заблокирована!"),
                          QSystemTrayIcon::Information,
                          kTrayHideTimeout);
#endif

#ifdef AUTO_UNLOCK_ENABLE
    trayIcon->showMessage(QString::fromUtf8("Внимание!"),
                          QString::fromUtf8("Разблокировка через 7с"),
                          QSystemTrayIcon::Information,
                          kTrayHideTimeout);
    mGoodHardwareSNs.clear();
    mBadHardwareSNs.clear();
    QTimer::singleShot(7000, this, SLOT(UnlockSystem()));
#endif
  }
}

void Core::SetCADActionsEnabled(bool enabled)
{
  int v = enabled ? 0 : 1;
  QSettings userSystem("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
                         QSettings::NativeFormat);
  userSystem.setValue("DisableChangePassword", v);
  userSystem.setValue("DisableLockWorkstation", v);
  userSystem.setValue("DisableTaskMgr", v);

  QSettings userExplorer("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
                         QSettings::NativeFormat);
  userExplorer.setValue("NoLogoff", v);
  userExplorer.setValue("NoClose", v);

  QSettings machineSystem("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
                         QSettings::NativeFormat);
  machineSystem.setValue("HideFastUserSwitching", v);
}

void Core::SetTrayIconAsLocked(bool locked)
{
  locked ? trayIcon->setIcon(mIconRed) : trayIcon->setIcon(mIconGreen);
}

void Core::SetTrayMessage(QString message)
{
#ifdef SHOW_TRAY_DEBUG
  trayIcon->setToolTip(message);
#endif

#ifdef SHOW_DEBUG
  qDebug() << message;
#endif
}



Core::Core(QObject *parent)
  : QObject(parent)
  , mSplash(new WidgetSplash())
  , mInfo(new DialogInfo(mSplash))
  , mFileWatcher(new QFileSystemWatcher())
  , mLastFileSize(0)
#ifdef Q_OS_UNIX
  , dpy(NULL)
#endif
  , mIsLocked(false)
{
  mIconGreen = QIcon(":/icons/lock-xxl-green.png");
  mIconRed = QIcon(":/icons/lock-xxl.png");
  CreateTrayIcon();
}

Core::~Core()
{
  mSplash->deleteLater();
  mFileWatcher->deleteLater();
}
