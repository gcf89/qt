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
#include <QStringList>

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
const char* kLineSeparator = "},{";

const QString kDevDisconnected = QString::fromUtf8("USB устройство извлечено");
const QString kDevAccepted = QString::fromUtf8("USB устройство разрешено");
const QString kDevRejected = QString::fromUtf8("USB устройство заблокировано");
const QString kDevConnected = QString::fromUtf8("USB устройство присоединено");
const QString kHVStarted = QString::fromUtf8("Драйвер гипервизора запущен");

//const QString kHWConnected = "c";
//const QString kHWDisconnected = "d";
//const QString kHWAccepted = "a";
//const QString kHWRejected = "r";


bool Core::Init(QString path)
{
  QFile f;
  if (path.isEmpty()) {
    QString confPath = qApp->applicationDirPath()+"/config";

    f.setFileName(confPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
      WriteDebug("ERROR: cannot open config file: " + confPath);
      return false;
    }
    QString data = f.readAll();
    QStringList config = data.split("\n", QString::SkipEmptyParts);
    f.close();

    if (config.isEmpty()) {
      WriteDebug("ERROR: empty config: " + confPath);
      return false;
    }

    path = config.first();

    if (config.size() == 5) {
      Gen::Instance().Width = config.at(1).toInt();
      Gen::Instance().Height = config.at(2).toInt();
      Gen::Instance().PosX = config.at(3).toInt();
      Gen::Instance().PosY = config.at(4).toInt();

      WriteDebug("Config gui: width " + config.at(1)
                 + "\nheight: " + config.at(2)
                 + "\nposx: " + config.at(3)
                 + "\nposy: " + config.at(4));
    }
  }

  // run before RunWatcher
  mSplash->PrepareGui();

  if (path.startsWith("./")) {
    path.remove(0, 1);
    path.prepend(qApp->applicationDirPath());
  }

  mSourcePath = path;
  WriteDebug("Source file: "+mSourcePath);

  f.setFileName(mSourcePath);
  if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
//    QTextStream in(&f);
//    in.setCodec(QTextCodec::codecForName("UTF8"));
//    QString data = in.readAll();
//    InitMandatoryHW(data);
//    mLastFileSize = f.size();

    RunWatcher(mSourcePath);
  } else {
    WriteDebug("ERROR: cannot open file:" + mSourcePath);
    return false;
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

  // like on start init
  onTargetFileChanged();
}

bool Core::Parse(QString data, qint64 filesize)
{
  Q_UNUSED(filesize)

  QStringList lines = data.simplified().split("},{");
  WriteDebug("Found lines: "+QString::number(lines.size()));

  int ind = -1;

  for (auto it=lines.constBegin(); it!=lines.constEnd(); ++it) {
    HW d = GetHWFrom(*it);


    if (d.text == kDevConnected) {
      if (!IsMandatory(d)) {
        mHWConnected << d;
        WriteDebug("!M connected: " + d.Str());
      } else {
        if ( (ind = mHWMandatoryDisconnected.indexOf(d)) != -1 ) {
          mHWMandatoryDisconnected.removeAt(ind);
          WriteDebug("M restored: " + d.Str());
        } else {
          WriteDebug("M reject: " + d.Str());
          mHWRejected << d;
        }
      }
    } else if (d.text == kDevDisconnected) {
      if (!IsMandatory(d)) {
        if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
          mHWConnected.removeAt(ind);
          WriteDebug("!M remove connected: " + d.Str());
        } else if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
          mHWRejected.removeAt(ind);
          WriteDebug("!M remove rejected: " + d.Str());
        } else if ( (ind = mHWAccepted.indexOf(d)) != -1 ) {
          mHWAccepted.removeAt(ind);
          WriteDebug("!M remove accepted: " + d.Str());
        } else {
          WriteDebug("??? !M remove (!connected): " + d.Str());
        }
      } else {
        if ( (ind = mHWRejected.indexOf(d)) != -1) {
          mHWRejected.removeAt(ind);
          WriteDebug("M remove rejected: " + d.Str());
        } else {
          mHWMandatoryDisconnected << d;
          WriteDebug("M disconnected: " + d.Str());
        }
      }
    } else if (d.text == kDevAccepted) {
      if (!IsMandatory(d)) {
        if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
          mHWConnected.removeAt(ind);
          mHWAccepted << d;
          WriteDebug("!M accept: " + d.Str());
        } else if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
            mHWRejected.removeAt(ind);
            mHWAccepted << d;
            WriteDebug("!M rejected became accepted without reconnection" + d.Str());
        } else {
          mHWAccepted << d;
          WriteDebug("??? !M accept (!connected, !rejected): " + d.Str());
        }
      } else {
        if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
          mHWRejected.removeAt(ind);
          WriteDebug("M (new) accepted: " + d.Str());
        } else {
          WriteDebug("??? M (new) missed connection stage (accept)" + d.Str());
        }
      }
    } else if (d.text == kDevRejected) {
      if (!IsMandatory(d)) {
        if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
          mHWConnected.removeAt(ind);
          mHWRejected << d;
          WriteDebug("!M reject: " + d.Str());
        } else if ( (ind = mHWAccepted.indexOf(d)) != -1 ) {
          mHWAccepted.removeAt(ind);
          mHWRejected << d;
          WriteDebug("!M accepted became rejected without reconnection" + d.Str());
        } else {
          mHWRejected << d;
          WriteDebug("??? !M reject (!connected): " + d.Str());
        }
      } else {
        if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
          WriteDebug("M already rejected: " + d.Str());
        } else if ( (ind = mHWMandatoryDisconnected.indexOf(d)) != -1 ) {
          mHWMandatoryDisconnected.removeAt(ind);
          WriteDebug("M accepted became rejected: " + d.Str());
        } else {
          mHWRejected << d;
          WriteDebug("??? M (new) missed connection stage (reject): " + d.Str());
        }
      }
    } else {
      WriteDebug("PASS " + d.Str());
    }
  }



//  int ind = -1;

//  for (auto it=lines.constBegin(); it!=lines.constEnd(); ++it) {
//    HW d = GetHWFrom(*it);
//    if (d.text == kDevConnected) {
//      if (mIfClassIdIgnore.contains(d.if_class_id)) {
//        mHWConnected << d;
//        WriteDebug("Connected: " + d.Str());
//      } else {
//        mHWRejected << d;
//        WriteDebug("Connected (auto reject): " + d.Str());
//      }
//    } else if (d.text == kDevDisconnected) {
//      if (mIfClassIdIgnore.contains(d.if_class_id)) {
//        WriteDebug("Not mandatory removed: " + d.Str());
//        if ( (ind = mHWRejected.indexOf(d)) != -1) {
//          mHWRejected.removeAt(ind);
//          WriteDebug("Not mandatory rejected removed: " + d.Str());
//        } else if ( (ind = mHWAccepted.indexOf(d)) != -1 ) {
//          mHWAccepted.removeAt(ind);
//          WriteDebug("Not mandatory accepted removed: " + d.Str());
//        }
//      } else {
//        if ( (ind = mHWMandatoryConnected.indexOf(d)) != -1 ) {
//          mHWMandatoryConnected.removeAt(ind);
//          mHWMandatoryDisconnected << d;
//          WriteDebug("Mandatory removed: " + d.Str());
//        } else if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
//          mHWRejected.removeAt(ind);
//          WriteDebug("Rejected removed: " + d.Str());
//        } else if ( (ind = mHWAccepted.indexOf(d)) != -1 ) {
//          // sane check
//          mHWAccepted.removeAt(ind);
//          WriteDebug("Accepted removed: " + d.Str());
//        } else {
//          mHWMandatoryDisconnected << d;
//          WriteDebug("Mandatory (not in the list) removed: " + d.Str());
//        }
//      }
//    } else if (d.text == kDevAccepted) {
//      if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
//        mHWConnected.removeAt(ind);
//        mHWAccepted << d;
//        WriteDebug("Accepted: " + d.Str());
//      } else if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
//        if (!mIfClassIdIgnore.contains(d.if_class_id)) { // maybe new accepted mandatory
//          mHWRejected.removeAt(ind);
//          mHWMandatoryConnected << d;
//          WriteDebug("? new mandatory: " + d.Str());
//        }
//      } else {
//        mHWAccepted << d;
//        WriteDebug("Accepted (not prev connected): " + d.Str());
//      }
//    } else if (d.text == kDevRejected) {
//      if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
//        mHWConnected.removeAt(ind);
//        WriteDebug("Reject connected: " + d.Str());
//      } else if ( (ind = mHWMandatoryConnected.indexOf(d)) != -1 ) {
//        mHWMandatoryConnected.removeAt(ind);
//        WriteDebug("Reject prev manadatory: " + d.Str());
//      } else if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
//        mHWRejected.removeAt(ind);
//      } else {
//        WriteDebug("Rejected without connection: " + d.Str());
//      }
//      mHWRejected << d;
//    } else {
//      WriteDebug("PASS " + d.Str() + " " + d.text);
//    }
//  }

  ConsiderLock();
  return true;
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
  if (!mHWAccepted.isEmpty()) {
    trayIcon->setToolTip(mHWAccepted.last().Msg());
  }

  bool needLock = !mHWMandatoryDisconnected.isEmpty() || !mHWRejected.isEmpty();

  QString msg;
  if (needLock) {
    if (!mHWMandatoryDisconnected.isEmpty()) {
      msg.append(tr("Список извлеченного основного оборудования:"));
    }
    for (auto it=mHWMandatoryDisconnected.constBegin(); it!=mHWMandatoryDisconnected.constEnd(); ++it) {
      msg.append("\n").append(it->Msg());
    }

    // separator
    if (!mHWMandatoryDisconnected.isEmpty()) {
      msg.append("\n\n");
    }

    if (!mHWRejected.isEmpty()) {
      msg.append(tr("Список вставленных запрещенных устройств:"));
    }
    for (auto it=mHWRejected.constBegin(); it!=mHWRejected.constEnd(); ++it) {
      msg.append("\n").append(it->Msg());
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
    WriteDebug("Current size: "
               + QString::number(f.size())
               + "Last size: "
               + QString::number(mLastFileSize));

  } else if (mLastFileSize < f.size()) {

    if (in.seek(mLastFileSize)) {
      WriteDebug("Check for new data: "
                 + QString::number(mLastFileSize)
                 + ", "
                 + QString::number(f.size()));

      if (Parse(in.read(f.size() - mLastFileSize), f.size())) {
        mLastFileSize = f.size();

        WriteDebug("Parse OK: "
                   + QString::number(mLastFileSize)
                   + ", "
                   + QString::number(f.size()));
      } else {
        WriteDebug("Parse FAILED: "
                   + QString::number(mLastFileSize)
                   + ", "
                   + QString::number(f.size()));
      }
    } else {
      WriteDebug("Error: 'seek' failed");
    }
  } else {
    WriteDebug("File size is smaller than expected: "
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
#ifndef SHOW_TRAY_DEBUG
#ifndef SHOW_DEBUG
  Q_UNUSED(message)
#endif
#endif

#ifdef SHOW_TRAY_DEBUG
  trayIcon->setToolTip(message);
#endif

//#ifdef SHOW_DEBUG
//  qDebug() << message;
//#endif
}

HW Core::GetHWFrom(const QString &line)
{
  int fromPos = 0;
  int toPos = 0;
  HW d;

  if ( (fromPos = line.indexOf( R"("id":)")) != -1 ) {
    if ( (toPos = line.indexOf(',', fromPos + 5)) != -1 ) {
      d.id = line.mid(fromPos + 5, toPos - fromPos - 5);
    }
  }
  if ( (fromPos = line.indexOf( R"("vendor_id":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 13)) != -1 ) {
      d.vendor_id = line.mid(fromPos + 13, toPos - fromPos - 13);
    }
  }
  if ( (fromPos = line.indexOf( R"("product_id":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 14)) != -1 ) {
      d.product_id = line.mid(fromPos + 14, toPos - fromPos - 14);
    }
  }
  if ( (fromPos = line.indexOf( R"("dev_class_id":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 16)) != -1 ) {
      d.dev_class_id = line.mid(fromPos + 16, toPos - fromPos - 16);
    }
  }
  if ( (fromPos = line.indexOf( R"("dev_subclass_id":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 19)) != -1 ) {
      d.dev_subclass_id = line.mid(fromPos + 19, toPos - fromPos - 19);
    }
  }
  if ( (fromPos = line.indexOf( R"("dev_protocol_id":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 19)) != -1 ) {
      d.dev_protocol_id = line.mid(fromPos + 19, toPos - fromPos - 19);
    }
  }
  if ( (fromPos = line.indexOf( R"("serial_number":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 17)) != -1 ) {
      d.serial_number = line.mid(fromPos + 17, toPos - fromPos - 17);
    }
  }
  if ( (fromPos = line.indexOf( R"("if_class_id":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 15)) != -1 ) {
      d.if_class_id = line.mid(fromPos + 15, toPos - fromPos - 15);
    }
  }
  if ( (fromPos = line.indexOf( R"("if_subclass_id":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 18)) != -1 ) {
      d.if_subclass_id = line.mid(fromPos + 18, toPos - fromPos - 18);
    }
  }
  if ( (fromPos = line.indexOf( R"("if_protocol_id":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 18)) != -1 ) {
      d.if_protocol_id = line.mid(fromPos + 18, toPos - fromPos - 18);
    }
  }
  if ( (fromPos = line.indexOf( R"("text":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 8)) != -1 ) {
      d.text = line.mid(fromPos + 8, toPos - fromPos - 8);
    }
  }
  if ( (fromPos = line.indexOf( R"("timestamp":")")) != -1 ) {
    if ( (toPos = line.indexOf('"', fromPos + 13)) != -1 ) {
      d.timestamp = line.mid(fromPos + 13, toPos - fromPos - 13);
    }
  }

  return d;
}

bool Core::IsMandatory(const HW &d) const
{
  return d.dev_class_id != "00" || !mIfClassIdIgnore.contains(d.if_class_id);
}

//void Core::InitMandatoryHW(QString &data)
//{
////  QFile f(mSourcePath);
////  if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
////    qDebug("Init mandaroty HW: cannot open "+mSourcePath);
////    exit(-1001);
////  }
////  QTextStream in(&f);
////  in.setCodec(QTextCodec::codecForName("UTF8"));

//  data = data.simplified();
//  QStringList lines = data.split(kLineSeparator);

//  WriteDebug("Found lines: "+QString::number(lines.size()));

//  for (auto it=lines.constBegin(); it!=lines.constEnd(); ++it) {
//    HW d = GetHWFrom(*it);

//    int ind = -1;

//    if (d.text == kDevConnected) {
//      if (!IsMandatory(d)) {
//        mHWConnected << d;
//        WriteDebug("!M connected: " + d.Str());
//      } else {
//        if ( (ind = mHWMandatoryDisconnected.indexOf(d)) != -1 ) {
//          mHWMandatoryDisconnected.removeAt(ind);
//          WriteDebug("M restored: " + d.Str());
//        } else {
//          WriteDebug("M reject: " + d.Str());
//          mHWRejected << d;
//        }
//      }
//    } else if (d.text == kDevDisconnected) {
//      if (!IsMandatory(d)) {
//        if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
//          mHWConnected.removeAt(ind);
//          WriteDebug("!M remove connected: " + d.Str());
//        } else if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
//          mHWRejected.removeAt(ind);
//          WriteDebug("!M remove rejected: " + d.Str());
//        } else if ( (ind = mHWAccepted.indexOf(d)) != -1 ) {
//          mHWAccepted.removeAt(ind);
//          WriteDebug("!M remove accepted: " + d.Str());
//        } else {
//          WriteDebug("??? !M remove (!connected): " + d.Str());
//        }
//      } else {
//        if ( (ind = mHWRejected.indexOf(d)) != -1) {
//          mHWRejected.removeAt(ind);
//          WriteDebug("M remove rejected: " + d.Str());
//        } else {
//          mHWMandatoryDisconnected << d;
//          WriteDebug("M disconnected: " + d.Str());
//        }
//      }
//    } else if (d.text == kDevAccepted) {
//      if (!IsMandatory(d)) {
//        if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
//          mHWConnected.removeAt(ind);
//          mHWAccepted << d;
//          WriteDebug("!M accept: " + d.Str());
//        } else if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
//            mHWRejected.removeAt(ind);
//            mHWAccepted << d;
//            WriteDebug("!M rejected became accepted without reconnection" + d.Str());
//        } else {
//          mHWAccepted << d;
//          WriteDebug("??? !M accept (!connected, !rejected): " + d.Str());
//        }
//      } else {
//        if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
//          mHWRejected.removeAt(ind);
//          WriteDebug("M (new) accepted: " + d.Str());
//        } else {
//          WriteDebug("??? M (new) missed connection stage (accept)" + d.Str());
//        }
//      }
//    } else if (d.text == kDevRejected) {
//      if (!IsMandatory(d)) {
//        if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
//          mHWConnected.removeAt(ind);
//          mHWRejected << d;
//          WriteDebug("!M reject: " + d.Str());
//        } else if ( (ind = mHWAccepted.indexOf(d)) != -1 ) {
//          mHWAccepted.removeAt(ind);
//          mHWRejected << d;
//          WriteDebug("!M accepted became rejected without reconnection" + d.Str());
//        } else {
//          mHWRejected << d;
//          WriteDebug("??? !M reject (!connected): " + d.Str());
//        }
//      } else {
//        if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
//          WriteDebug("M already rejected: " + d.Str());
//        } else if ( (ind = mHWMandatoryDisconnected.indexOf(d)) != -1 ) {
//          mHWMandatoryDisconnected.removeAt(ind);
//          WriteDebug("M accepted became rejected: " + d.Str());
//        } else {
//          mHWRejected << d;
//          WriteDebug("??? M (new) missed connection stage (reject): " + d.Str());
//        }
//      }
//    } else {
//      WriteDebug("PASS " + d.Str());
//    }






////    if (d.text == kDevConnected) {
////      if (mIfClassIdIgnore.contains(d.if_class_id)) {
////        mHWConnected << d;
////        WriteDebug("Connected: " + d.Str());
////      } else {
////        // если не получим accept || reject добавим сразу
////        mHWMandatoryConnected << d;
////        WriteDebug("Connected (mandatory): " + d.Str());
////      }
////    } else if (d.text == kDevDisconnected) {
////      if (mIfClassIdIgnore.contains(d.if_class_id)) {
////        if ( (ind = mHWRejected.indexOf(d)) != -1) {
////          mHWRejected.removeAt(ind);
////          WriteDebug("Not mandatory rejected removed: " + d.Str());
////        } else if ( (ind = mHWAccepted.indexOf(d)) != -1 ) {
////          mHWAccepted.removeAt(ind);
////          WriteDebug("Not mandatory accepted removed: " + d.Str());
////        } else if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
////          mHWConnected.removeAt(ind);
////          WriteDebug("Not mandatory removed: " + d.Str());
////        } else {
////          WriteDebug("Not mandatory removed (!connected): " + d.Str());
////        }
////      } else {
////        if ( (ind = mHWMandatoryConnected.indexOf(d)) != -1 ) {
////          mHWMandatoryConnected.removeAt(ind);
////          mHWMandatoryDisconnected << d;
////          WriteDebug("Mandatory removed: " + d.Str());
////        } else if ( (ind = mHWRejected.indexOf(d)) != -1 ) {
////          mHWRejected.removeAt(ind);
////          WriteDebug("Rejected removed: " + d.Str());
////        } else if ( (ind = mHWAccepted.indexOf(d)) != -1 ) {
////          // sane check
////          mHWAccepted.removeAt(ind);
////          WriteDebug("Accepted removed: " + d.Str());
////        } else {
////          mHWMandatoryDisconnected << d;
////          WriteDebug("Mandatory (not in the list) removed: " + d.Str());
////        }
////      }
////    } else if (d.text == kDevAccepted) {
////      if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
////        mHWConnected.removeAt(ind);
////        WriteDebug("Accepted: " + d.Str());
////      } else {
////        WriteDebug("Accepted (not prev connected): " + d.Str());
////      }
////      mHWAccepted << d;
////    } else if (d.text == kDevRejected) {
////      int ind = -1;
////      if ( (ind = mHWConnected.indexOf(d)) != -1 ) {
////        mHWConnected.removeAt(ind);
////        WriteDebug("Reject connected: " + d.Str());
////      } else if ( (ind = mHWMandatoryConnected.indexOf(d)) != -1 ) {
////        mHWMandatoryConnected.removeAt(ind);
////        WriteDebug("Reject prev manadatory: " + d.Str());
////      } else {
////        WriteDebug("Rejected without connection: " + d.Str());
////      }
////      mHWRejected << d;
////    } else {
////      WriteDebug("PASS " + d.Str() + " " + d.text);
////    }

//  } // for

////  ConsiderLock();
//}



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
  mIfClassIdIgnore.insert("08"); // mass storage device
  mIfClassIdIgnore.insert("07"); // printer

  mIconGreen = QIcon(":/icons/lock-xxl-green.png");
  mIconRed = QIcon(":/icons/lock-xxl.png");
  CreateTrayIcon();
}

Core::~Core()
{
  mSplash->deleteLater();
  mFileWatcher->deleteLater();
}
