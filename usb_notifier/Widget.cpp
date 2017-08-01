#include <QApplication>
#include <QFile>
#include <QTextStream>
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QJsonArray>
//#include <QJsonParseError>
//#include <QJsonValue>
#include <QMessageBox>
#include <QTextCodec>
#include <QTimer>
#include <QThread>
#include <QStyleOption>

#ifdef Q_OS_WIN32
#include <windows.h>

UINT fuiPreviousState; // wtf?

HHOOK hKeyHook = NULL;
HHOOK hMouseHook = NULL;

LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return -1;
}
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
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

#ifdef Q_OS_UNIX

#endif


#include "Widget.h"
#include "ui_Widget.h"

// for debugging
//#define DBG


bool Widget::Load(QString path)
{
  QFile targetFile;
  if (path.startsWith("./")) {
    path.remove(0, 1);
    path.prepend(qApp->applicationDirPath());
  }
  targetFile.setFileName(path);
  if (!targetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    targetFile.setFileName(qApp->applicationDirPath()+"/config");
    if (!targetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qFatal("Cannot open config file. Exit!");
    }
    QString pathFromConfig = targetFile.readAll();
    targetFile.close();
    if (pathFromConfig.startsWith("./")) {
      pathFromConfig.remove(0, 1);
      pathFromConfig.prepend(qApp->applicationDirPath());
    }
    targetFile.setFileName(pathFromConfig);
    if (targetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      RunWatcher(pathFromConfig);
    } else {
      return false;
    }
  } else {
    RunWatcher(path);
  }

  return true;
}

void Widget::setVisible(bool visible)
{
  minimizeAction->setEnabled(visible);
  maximizeAction->setEnabled(!isMaximized());
  restoreAction->setEnabled(isMaximized() || !visible);
  QWidget::setVisible(visible);
}

void Widget::closeEvent(QCloseEvent *event)
{
  if (trayIcon->isVisible()) {
//      QMessageBox::information(this, tr("Systray"),
//                               tr("The program will keep running in the "
//                                  "system tray. To terminate the program, "
//                                  "choose <b>Quit</b> in the context menu "
//                                  "of the system tray entry."));
      hide();
      event->ignore();
  }
}

void Widget::RunWatcher(const QString& path)
{
  mSourcePath = path;

  QFile f(mSourcePath);
  mFilePos = f.size();

  mFileWatcher->addPath(mSourcePath);
  connect(mFileWatcher, SIGNAL(fileChanged(QString)), SLOT(onTargetFileChanged()));
}

bool Widget::Parse(QString data, qint64 filesize)
{
  QString id;
  QString serialNumber;
  QString text;
  QString timestamp;

  data = data.simplified();

  int fromPos = 0;
  int toPos = 0;

  if ( (fromPos = data.lastIndexOf("\"id\"")) != -1 ) {
    if ( (toPos = data.indexOf(",", fromPos)) != -1 ) {
      id = data.mid(fromPos, toPos - fromPos);

      if ( (fromPos = data.lastIndexOf("\"serial_number\"")) != -1 ) {
        if ( (toPos = data.indexOf("\"}", fromPos)) != -1 ) {
          serialNumber = data.mid(fromPos, toPos - fromPos);

          if ( (fromPos = data.lastIndexOf("\"text\"")) != -1 ) {
            if ( (toPos = data.indexOf("\",", fromPos)) != -1 ) {
              text = data.mid(fromPos, toPos - fromPos);

              if ( (fromPos = data.lastIndexOf("\"timestamp\"")) != -1 ) {
                if ( (toPos = data.indexOf("\"}", fromPos)) != -1 ) {
                  timestamp = data.mid(fromPos, toPos - fromPos);

                  QString html = QString("<p><b>ID</b>: %0</p>"
                                         "<p><b>Серийный номер</b>: %1</p>"
                                         "<p><b>Сообщение</b>: %2</p>"
                                         "<p><b>Дата</b>: %3</p>")
                      .arg(id.section(":",1,1))
                      .arg(serialNumber.section(":",1,1).replace(0,1,""))
                      .arg(text.section(":",1,1).replace(0,1,""))
                      .arg(timestamp.section(":",1,1).replace(0,1,""));

// old notify style
//                  show();

//                  ui->textEdit->setHtml(html);

//                  trayIcon->showMessage(QObject::trUtf8("Внимание!"),
//                                        QObject::trUtf8("Система заблокирована!"),
//                                        QSystemTrayIcon::Information,
//                                        2000);

//                  if ( text.section(":",1,1).replace(0,1,"") == "block" ) {
//                    BlockSystem();
//                  } else if ( text.section(":",1,1).replace(0,1,"") == "unblock" ) {
//                    UnblockSystem();
//                  }

                  // if bad event
                  // grab keyboard
                  // grab mouse
                  // show message

                  // if good event
                  // reverse

                  // for testing
                  // if good, disable lock hook

                  return true;

                } else {
                  qDebug() << "Failed to find TIMESTAMP end!";
                }
              } else {
                qDebug() << "Failed to find TIMESTAMP start!";
              }
            } else {
              qDebug() << "Failed to find TEXT end!";
            }
          } else {
            qDebug() << "Failed to find TEXT start!";
          }
        } else {
          qDebug() << "Failed to find SERIAL end!";
        }
      } else {
        qDebug() << "Failed to find SERIAL start!";
      }
    } else {
      qDebug() << "Failed to find ID end!";
    }
  } else {
    qDebug() << "Failed to find ID start!";
  }
  return false;
}

void Widget::CreateActions()
{
  minimizeAction = new QAction(tr("Mi&nimize"), this);
  connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

  maximizeAction = new QAction(tr("Ma&ximize"), this);
  connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

  restoreAction = new QAction(tr("&Restore"), this);
  connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

  quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Widget::CreateTrayIcon()
{
  trayIconMenu = new QMenu(this);
  trayIconMenu->addAction(minimizeAction);
  trayIconMenu->addAction(maximizeAction);
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(QIcon(":/icons/GreenCircle600.png"), this);
  trayIcon->setContextMenu(trayIconMenu);
}

void Widget::OldGuiEnabled(bool enabled)
{
  ui->label->setVisible(enabled);
  ui->pushButton->setVisible(enabled);
  ui->textEdit->setVisible(enabled);
}

void Widget::NewGuiEnabled(bool enabled)
{
  ui->labelWarn->setVisible(enabled);
  ui->labelWarn->setText(QString::fromUtf8("Устройство заблокировано"));
  setWindowFlags(Qt::FramelessWindowHint);
  setStyleSheet("QLabel { background-color: #ff6e6e;"
                "color: #c41010; }");
}

void Widget::onTargetFileChanged()
{
  QFile f(mSourcePath);
  if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qCritical("Error: cannot open target file!");
    return;
  }
  QTextStream in(&f);
  in.setCodec(QTextCodec::codecForName("UTF8"));


  if (mFilePos == f.size()) {
    qDebug() << "File changed, but size still the same!";
  } else if (mFilePos < f.size()) {

    if (in.seek(mFilePos)) {
      if (Parse(in.read(f.size() - mFilePos), f.size())) {
        mFilePos = f.size();
      }
    } else {
      qDebug() << "bad seek";
    }
  } else {
    // rotation: some data could be lost
    if (Parse(in.readAll(), f.size())) {
      mFilePos = f.size();
    }
  }

#ifdef DBG
  ui->label->setText(QString("File pos: %0, File size: %1")
                     .arg(QString::number(mFilePos))
                     .arg(QString::number(f.size())));
#endif
}

void Widget::on_pushButton_clicked()
{
  hide();
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
//  switch (reason) {
//  case QSystemTrayIcon::Trigger:
//  case QSystemTrayIcon::DoubleClick:
//    iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
//    break;
//  case QSystemTrayIcon::MiddleClick:
//    showMessage();
//    break;
//  default:
//    ;
  //  }
}

void Widget::UnlockSystem()
{
#ifdef Q_OS_WIN32
  UnlockSpecialKeys(&fuiPreviousState);

  UnhookWindowsHookEx(hKeyHook);
  UnhookWindowsHookEx(hMouseHook);
#endif

#ifdef Q_OS_UNIX
  XUngrabKeyboard(dpy, CurrentTime);
  XUngrabPointer(dpy, CurrentTime);

  if (XCloseDisplay(dpy)) {
     qDebug() << Q_FUNC_INFO << "error XCloseDisplay";
  }
#endif

  hide();
  trayIcon->showMessage(QString::fromUtf8("Внимание!"),
                        QString::fromUtf8("Система разблокирована!"),
                        QSystemTrayIcon::Information,
                        2000);
}

void Widget::LockSystem()
{
#ifdef Q_OS_WIN32
  LockSpecialKeys(&fuiPreviousState);

//  hKeyHook = SetWindowsHookEx(WH_KEYBOARD, KeyProc, NULL, 0);
  hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (KeyProc), 0, 0);
  hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (MouseProc), NULL, 0);
#endif

#ifdef Q_OS_UNIX
      XEvent ev;
      char *s;
      unsigned int kc;

      if (NULL==(dpy=XOpenDisplay(NULL))) {
         qDebug() << Q_FUNC_INFO << "error XOpenDisplay";
      }

      XGrabKeyboard(dpy, DefaultRootWindow(dpy),
                    true, GrabModeAsync, GrabModeAsync, CurrentTime);

//      XGrabPointer(dpy, DefaultRootWindow(dpy), false, 0,  GrabModeSync);
      XGrabPointer(dpy, DefaultRootWindow(dpy), 0, 0, 0, 0, DefaultRootWindow(dpy), None, CurrentTime);

//      bool ctrlmodified = false;
//      bool shiftmodified = false;

//      while(hk->isActive()) {
//         XNextEvent(dpy, &ev);
//         switch (ev.type) {
//            case KeyPress:
//               kc = ((XKeyPressedEvent*)&ev)->keycode;
//               s = XKeysymToString(XKeycodeToKeysym(dpy, kc, 0));
//               qDebug() << Q_FUNC_INFO << s;
//               if (!strcmp(s, "Control_L")) ctrlmodified=true;
//               if (!strcmp(s, "Control_R")) ctrlmodified=true;
//               if (!strcmp(s, "Shift_L")) shiftmodified=true;
//               if (!strcmp(s, "Shift_R")) shiftmodified=true;

//               if (!strcmp(s, "Print") && ctrlmodified && shiftmodified) {
//                  hk->press(HookKeybord::Print | HookKeybord::Shift | HookKeybord::Ctrl);
//                  continue;
//               }

//               if (!strcmp(s, "Print") && ctrlmodified) {
//                  hk->press(HookKeybord::Print | HookKeybord::Ctrl);
//                  continue;
//               }

//               if (!strcmp(s, "Print") && shiftmodified) {
//                  hk->press(HookKeybord::Print | HookKeybord::Shift);
//                  continue;
//               }

//               if (!strcmp(s, "Print")) {
//                  hk->press(HookKeybord::Print);
//                  continue;
//               }

//               XSendEvent(dpy, 0, false, KeyPressMask,&ev);
//               break;
//            case Expose:
//                  while (XCheckTypedEvent(dpy, Expose, &ev)) /* empty body */ ;
//               break;
//            case ButtonPress:
//            case ButtonRelease:
//            case KeyRelease:
//               kc = ((XKeyReleasedEvent*)&ev)->keycode;
//               s = XKeysymToString(XKeycodeToKeysym(dpy, kc, 0));


//               if (!strcmp(s, "Control_L")) ctrlmodified=false;
//               if (!strcmp(s, "Control_R")) ctrlmodified=false;
//               if (!strcmp(s, "Shift_L")) shiftmodified=false;
//               if (!strcmp(s, "Shift_R")) shiftmodified=false;
//            case MotionNotify:
//            case ConfigureNotify:
//            default:
//               break;
//         }
//      }


#endif

  show();
  trayIcon->showMessage(QString::fromUtf8("Внимание!"),
                        QString::fromUtf8("Система заблокирована!"),
                        QSystemTrayIcon::Information,
                        2000);
}


Widget::Widget(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::Widget)
  , mFileWatcher(new QFileSystemWatcher())
  , mFilePos(0)
  , dpy(NULL)
{
  ui->setupUi(this);

#ifndef DBG
  ui->label->setVisible(false);
#endif

  CreateActions();
  CreateTrayIcon();

  connect(trayIcon, &QSystemTrayIcon::activated, this, &Widget::iconActivated);

  trayIcon->show();


  OldGuiEnabled(false);
  NewGuiEnabled(true);


  LockSystem();
  QTimer::singleShot(7000, this, SLOT(UnlockSystem()));
}

Widget::~Widget()
{
  delete ui;
}
