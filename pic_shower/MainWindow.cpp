#include <algorithm>

#include <QFile>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QList>

#include "MainWindow.h"
#include "ui_MainWindow.h"

// emulate com port on linux:
// https://stackoverflow.com/questions/52187/virtual-serial-port-for-linux

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  mDirWatcher.reset(new QFileSystemWatcher());
  connect(mDirWatcher.data(), SIGNAL(directoryChanged(QString)),
          this, SLOT(OnDirChanged(QString)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

bool MainWindow::ReadSettings()
{
  mSettings.ReadConfig();
  return mSettings.IsValid();
}

bool MainWindow::Init()
{
  mDirWatcher->addPath(mSettings.imageDir);
  qDebug() << "Image dir:" << mSettings.imageDir;

  if (mSettings.showMaximized) {
    showMaximized();
  }
  qDebug() << "Show maximized:" << mSettings.showMaximized;

  if ( (mSettings.buttonHeight != 0) && (mSettings.buttonWidth != 0) ) {
    ui->pushButtonAccept->setMinimumSize(mSettings.buttonWidth,
                                         mSettings.buttonHeight);
    ui->pushButtonReject->setMinimumSize(mSettings.buttonWidth,
                                         mSettings.buttonHeight);
    qDebug() << "Button width:" << mSettings.buttonWidth
             << "Button height:" << mSettings.buttonHeight;
  } else {
    qDebug() << "Button size: default";
  }

  QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
  qDebug() << "Available ports:";
  foreach (auto pi, availablePorts) {
    qDebug() << pi.portName();
  }

  std::find_if(availablePorts.constBegin(), availablePorts.constEnd(),
  [&](const QSerialPortInfo& pi) -> bool {
//    if (mSettings.portName == pi.portName()) {
      return mSettings.portName == pi.portName();
//    }
  });

//  if (!availablePorts.contains(mSettings.portName)) {
//    qCritical() << "Cannot find port specified:" << mSettings.portName;
//    return false;
//  }
  qWarning() << "OK: port found!";
  return true;
}

bool MainWindow::CacheExistingFileNames()
{
  return true;
}

void MainWindow::on_pushButtonAccept_clicked()
{
  //
}

void MainWindow::on_pushButtonReject_clicked()
{
  //
}

void MainWindow::OnDirChanged(QString path)
{
  Q_UNUSED(path)

  qDebug() << "D: dir changed";

  QDir d(mSettings.imageDir);
  QStringList fileNames = d.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg");

  for (auto it=fileNames.constBegin(); it!=fileNames.constEnd(); ++it) {
    qDebug() << *it;
  }

  if (!fileNames.isEmpty()) {
    QString picPath = qApp->applicationDirPath()+"/pics/"+fileNames.last();
//    QString picPath = fileNames.last();
    qDebug() << picPath;

    QPixmap p;
    if (p.load(picPath)) {
      ui->labelPicture->setPixmap(p);
    } else {
      qDebug() << "ERR: cannot load picture";
    }
  }
}
