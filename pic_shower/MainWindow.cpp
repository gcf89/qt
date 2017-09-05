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
  qInfo() << "Image dir:" << mSettings.imageDir;
  if (mSettings.imageDir.startsWith("./")) {
    mSettings.imageDir = qApp->applicationDirPath()+QDir::separator()+mSettings.imageDir.mid(2);
  }
  mDirWatcher->addPath(mSettings.imageDir);

  qInfo() << "Show maximized:" << mSettings.showMaximized;
  if (mSettings.showMaximized) {
    setWindowFlags(Qt::FramelessWindowHint);
    showMaximized();
  }

  qInfo() << "Show pic fullsize:" << mSettings.imageShowFullSize;
  ui->labelPicture->setScaledContents(mSettings.imageShowFullSize);

  if ( (mSettings.buttonHeight != 0) && (mSettings.buttonWidth != 0) ) {
    ui->pushButtonAccept->setMinimumSize(mSettings.buttonWidth,
                                         mSettings.buttonHeight);
    ui->pushButtonReject->setMinimumSize(mSettings.buttonWidth,
                                         mSettings.buttonHeight);
    qInfo() << "Button width:" << mSettings.buttonWidth
             << "Button height:" << mSettings.buttonHeight;
  } else {
    qInfo() << "Button size: default";
  }

  qInfo() << "Available ports:";
  QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
  foreach (auto pi, availablePorts) {
    qInfo() << pi.portName();
  }

  auto it = std::find_if(availablePorts.constBegin(), availablePorts.constEnd(),
                         [&](const QSerialPortInfo& pi) -> bool {
    return mSettings.portName == pi.portName();
  });

  if (it != availablePorts.constEnd()) {
    qInfo() << "Port found:" << mSettings.portName;
    return true;
  } else {
    qCritical() << "Port NOT found:" << mSettings.portName;
    return false;
  }
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

  qDebug() << "Dir changed:" << path;

  QDir d(mSettings.imageDir);
  QStringList fileNames = d.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg");

  qDebug() << "Pics in dir:";
  for (auto it=fileNames.constBegin(); it!=fileNames.constEnd(); ++it) {
    qDebug() << "Pic:" << *it;
  }

  if (!fileNames.isEmpty()) {
    QString picPath = mSettings.imageDir+QDir::separator()+fileNames.last();
    qDebug() << "Show pic:" << picPath;

    QPixmap p;
    if (p.load(picPath)) {
      ui->labelPicture->setPixmap(p);

    } else {
      qCritical() << "cannot load picture";
    }
  }
}
