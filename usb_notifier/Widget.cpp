#include <limits>

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>
#include <QTimer>
#include <QThread>
#include <QStyleOption>
#include <QDesktopWidget>

#ifdef Q_OS_WIN32
#include <windows.h>

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
  Q_UNUSED(filesize)

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

                  // old notify style

//                  QString html = QString("<p><b>ID</b>: %0</p>"
//                                         "<p><b>Серийный номер</b>: %1</p>"
//                                         "<p><b>Сообщение</b>: %2</p>"
//                                         "<p><b>Дата</b>: %3</p>")
//                      .arg(id.section(":",1,1))
//                      .arg(serialNumber.section(":",1,1).replace(0,1,""))
//                      .arg(text.section(":",1,1).replace(0,1,""))
//                      .arg(timestamp.section(":",1,1).replace(0,1,""));


//                  show();

//                  ui->textEdit->setHtml(html);

//                  trayIcon->showMessage(QObject::trUtf8("Внимание!"),
//                                        QObject::trUtf8("Система заблокирована!"),
//                                        QSystemTrayIcon::Information,
//                                        2000);

                  QString sn = serialNumber.section(":",1,1).replace(0,1,"");
                  QString msg = text.section(":",1,1).replace(0,1,"");


                  if ( msg == "gr" ) {
                    mGoodHardwareSNs << sn;
                  } else if ( msg == "gi" ) {
                    mGoodHardwareSNs.removeOne(sn);
                  } else if ( msg == "bi" ) {
                    mBadHardwareSNs << sn;
                  } else if ( msg == "br" ) {
                    mBadHardwareSNs.removeOne(sn);
                  }

                  ConsiderLock();

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
  minimizeAction = new QAction(tr("Скрыть"), this);
  connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

  maximizeAction = new QAction(tr("Развернуть"), this);
  connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

  restoreAction = new QAction(tr("Восстановить"), this);
  connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

  quitAction = new QAction(tr("Выход"), this);
  connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Widget::CreateTrayIcon()
{
  trayIconMenu = new QMenu(this);
//  trayIconMenu->addAction(minimizeAction);
//  trayIconMenu->addAction(maximizeAction);
//  trayIconMenu->addAction(restoreAction);
//  trayIconMenu->addSeparator();
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
  setWindowFlags(Qt::FramelessWindowHint);
  setStyleSheet("QLabel { background-color: red;"
                "color: black; }");
}

void Widget::CenterWindow()
{
  QDesktopWidget desktop;
  QRect rect = desktop.availableGeometry(desktop.primaryScreen()); // прямоугольник с размерами экрана
  QPoint center = rect.center(); //координаты центра экрана
  int x = center.x() - (this->width()/2);  // учитываем половину ширины окна
  int y = center.y() - (this->height()/2); // .. половину высоты
  center.setX(x);
  center.setY(y);
  this->move(center);
}

void Widget::ConsiderLock()
{
  QString msg;
  bool needLock = false;

  if (!mGoodHardwareSNs.isEmpty() || !mBadHardwareSNs.isEmpty()) {
    msg = tr("Комплект нарушен.");
    if (!mGoodHardwareSNs.isEmpty()) {
      msg.append(tr("\n\nСписок извлеченного основного оборудования:"));
    }
    for (auto it=mGoodHardwareSNs.constBegin(); it!=mGoodHardwareSNs.constEnd(); ++it) {
      msg.append("\n").append(*it);
    }

    if (!mBadHardwareSNs.isEmpty()) {
      msg.append(tr("\n\nСписок вставленных запрещенных устройств:"));
    }
    for (auto it=mBadHardwareSNs.constBegin(); it!=mBadHardwareSNs.constEnd(); ++it) {
      msg.append("\n").append(*it);
    }

    needLock = true;

  } else {
    msg = QString::fromUtf8("Комплект восстановлен.");
  }

  ui->labelWarn->setText(msg);

  if (needLock) {
    LockSystem();
  } else {
    UnlockSystem();
  }
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
  Q_UNUSED(reason)

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
  mIsLocked = false;

//#ifdef Q_OS_WIN32
//  UnlockSpecialKeys(&fuiPreviousState);

//  UnhookWindowsHookEx(hKeyHook);
//  UnhookWindowsHookEx(hMouseHook);
//#endif

//#ifdef Q_OS_UNIX
//  XUngrabKeyboard(dpy, CurrentTime);
//  XUngrabPointer(dpy, CurrentTime);

//  if (XCloseDisplay(dpy)) {
//     qDebug() << Q_FUNC_INFO << "error XCloseDisplay";
//  }
//#endif

  trayIcon->showMessage(QString::fromUtf8("Внимание!"),
                        QString::fromUtf8("Система разблокирована!"),
                        QSystemTrayIcon::Information,
                        2000);

//  hide();
  QTimer::singleShot(1500, this, SLOT(hide()));
}

void Widget::LockSystem()
{
  if (!mIsLocked) {
    mIsLocked = true;

//#ifdef Q_OS_WIN32
//    LockSpecialKeys(&fuiPreviousState);

//    hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (KeyProc), 0, 0);
//    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (MouseProc), NULL, 0);
//#endif

//#ifdef Q_OS_UNIX
//    if (NULL==(dpy=XOpenDisplay(NULL))) {
//      qDebug() << Q_FUNC_INFO << "error XOpenDisplay";
//    }

//    XGrabKeyboard(dpy, DefaultRootWindow(dpy), true, GrabModeAsync, GrabModeAsync, CurrentTime);

//    XGrabPointer(dpy, DefaultRootWindow(dpy), 0, 0, 0, 0, DefaultRootWindow(dpy), None, CurrentTime);
//#endif

    show();
    activateWindow();

    trayIcon->showMessage(QString::fromUtf8("Внимание!"),
                          QString::fromUtf8("Система заблокирована! Откл. через 7с"),
                          QSystemTrayIcon::Information,
                          2000);

    QTimer::singleShot(7000, this, SLOT(UnlockSystem()));
  }
}


Widget::Widget(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::Widget)
  , mFileWatcher(new QFileSystemWatcher())
  , mFilePos(0)
#ifdef Q_OS_UNIX
  , dpy(NULL)
#endif
  , mIsLocked(false)
{
  ui->setupUi(this);

  // resize to normal window
//  resize(QDesktopWidget().availableGeometry(this).size() * 0.3);

  // resize to max (dirty hack)
//  resize(32766, 2048);

  // resize to stripe
  resize(32766, 300);

  // flags
  setWindowFlags(Qt::WindowStaysOnTopHint);

  CenterWindow();

  CreateActions();
  CreateTrayIcon();

  connect(trayIcon, &QSystemTrayIcon::activated, this, &Widget::iconActivated);

  trayIcon->show();

  // gui
  OldGuiEnabled(false);
  NewGuiEnabled(true);

  // auto hide
  QTimer::singleShot(0, this, SLOT(hide()));

#ifndef DBG
  ui->label->setVisible(false);
#endif
}

Widget::~Widget()
{
  delete ui;
}
