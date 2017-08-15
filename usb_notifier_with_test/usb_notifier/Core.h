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
#include <QSet>
#include <QMultiMap>

#ifdef Q_OS_UNIX
#include <X11/Xlib.h>
#endif

#include "WidgetSplash.h"
#include "DialogInfo.h"


/*
 * 1. read hvdat to get initial good hw
 * 2. if inserted and good add to tmp list (usb)
 * 3. if inserted and bad lock
 * 4. if removed good from tmp list (ok, notify)
 * 5. if removed good from initial list (lock)
 */

class HW
{
public:
  static bool IsIgnored(const HW& d) {
    return HW::IsUsbHub(d);
  }

public:
  QString id;
  QString vendor_id;
  QString product_id;
  QString dev_class_id;
  QString dev_subclass_id;
  QString dev_protocol_id;
  QString serial_number;
  QString if_class_id;
  QString if_subclass_id;
  QString if_protocol_id;
  QString text;
  QString timestamp;

public:
  QString Str() const {
    QString res = IsParseOk() ? "[OK]  " : "[BAD] ";
    return res
        + "Id: " + id
        + " Vendor: " + vendor_id
        + " Product: " + product_id
        + " Dev class: " + dev_class_id
        + " Dev subclass: " + dev_subclass_id
        + " Dev prot: " + dev_protocol_id
        + " Serial: " + serial_number
        + " Iface class: " + if_class_id
        + " Iface subclass: " + if_subclass_id
        + " Iface prot: " + if_protocol_id
        + " Text: " + text
        + " Timestamp: " + timestamp
        ;
  }

  bool IsParseOk() const {
    return !id.isEmpty()
        && !vendor_id.isEmpty()
        && !product_id.isEmpty()
        && !dev_class_id.isEmpty()
        && !dev_subclass_id.isEmpty()
        && !dev_protocol_id.isEmpty()
        && !if_class_id.isEmpty()
        && !if_subclass_id.isEmpty()
        && !if_protocol_id.isEmpty()
        && !text.isEmpty()
        && !timestamp.isEmpty();
  }

  QString Msg() const {
    QString msg;
    if (IsKeyboard(*this)) {
      msg = "Устройство ввода (клавиатура) " + MsgSN();
    } else if (IsMouse(*this)) {
      msg = "Устройство ввода (мышь) " + MsgSN();
    } else if (IsMassStorage(*this)) {
      msg = "Накопитель " + MsgSN();
    } else if (IsUsbHub(*this)) {
      msg = "Устройство ввода (USB Hub) " + MsgSN();
    } else {
      msg = "Устройство " + FirstNotZeroId() + " " + MsgSN();
    }
    return msg;
  }

  QString FirstNotZeroId() const {
    if (dev_class_id == "00" || dev_class_id.isEmpty()) {
      if (dev_subclass_id == "00" || dev_subclass_id.isEmpty()) {
        if (if_class_id == "00" || if_class_id.isEmpty()) {
          if (if_subclass_id == "00" || if_subclass_id.isEmpty()) {
            return vendor_id;
          } else {
            return if_subclass_id;
          }
        } else {
          return if_class_id;
        }
      } else {
        return dev_subclass_id;
      }
    } else {
      return dev_class_id;
    }
  }

  QString MsgSN() const {
    if (!serial_number.isEmpty()) {
      return "(SN: " + serial_number + ")";
    } else {
      return "";
    }
  }

  static bool IsKeyboard(const HW& d) {
    return d.if_class_id == "03"
        && d.if_subclass_id == "01"
        && d.if_protocol_id == "01";
  }

  static bool IsMouse(const HW& d) {
    return d.if_class_id == "03"
        && d.if_subclass_id == "01"
        && d.if_protocol_id == "02";
  }

  static bool IsUsbHub(const HW& d) {
    return d.if_class_id == "09"
        && d.if_subclass_id == "00";
  }

  static bool IsMassStorage(const HW& d) {
    return d.if_class_id == "08"
        && d.if_subclass_id == "06";
  }
};

inline bool operator==(const HW& d1, const HW& d2)
{
  return d1.vendor_id == d2.vendor_id
      && d1.product_id == d2.product_id
      && d1.dev_class_id == d2.dev_class_id
      && d1.dev_subclass_id == d2.dev_subclass_id
      && d1.dev_protocol_id == d2.dev_protocol_id
      && d1.serial_number == d2.serial_number
      && d1.if_class_id == d2.if_class_id
      && d1.if_subclass_id == d2.if_subclass_id
      && d1.if_protocol_id == d2.if_protocol_id
      ;
}


class Core : public QObject
{
  Q_OBJECT
public:
  WidgetSplash*               mSplash;
  DialogInfo*                 mInfo;
  QFileSystemWatcher*         mFileWatcher;
  qint64                      mLastFileSize;
  QAction*                    quitAction;
  QSystemTrayIcon*            trayIcon;
  QMenu*                      trayIconMenu;
  QString                     mSourcePath;
//  QStringList                 mHWGood;
//  QStringList                 mHWInstalledRemoved;
//  QStringList                 mHWBad;
#ifdef Q_OS_UNIX
  Display*                    dpy;
#endif
  bool                        mIsLocked;
  QIcon                       mIconGreen;
  QIcon                       mIconRed;

  QSet<QString>               mIfClassIdIgnore;
  QList<HW>                   mHWMandatoryConnected;
  QList<HW>                   mHWMandatoryDisconnected;
//  QMultiMap<QString, HW>      mRegisteredHW;
  QList<HW>                   mHWAccepted;
  QList<HW>                   mHWRejected;
  QList<HW>                   mHWConnected;
  bool                        mHasKeyboard;
  bool                        mHasMouse;

public:
  bool Init(QString path);

public slots:
  void RunWatcher();
public:
  bool Parse(QString data, qint64 filesize);
  void CreateTrayIcon();
  void ConsiderLock();
private slots:
  void UnlockSystem();
public:
  void LockSystem();
  void SetCADActionsEnabled(bool enabled);
  void SetTrayIconAsLocked(bool locked);
  void SetTrayMessage(QString message);

public: // private
  HW GetHWFrom(const QString& line);
  bool IsMandatory(const HW& d) const;
//  void InitMandatoryHW(QString& data);

private slots:
  void onTargetFileChanged();

public:
  explicit Core(QObject *parent = 0);
  ~Core();
};
