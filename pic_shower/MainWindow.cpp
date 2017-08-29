#include <QFileSystemWatcher>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QList>

#include "MainWindow.h"
#include "ui_MainWindow.h"

// emulate com port on linux: https://stackoverflow.com/questions/52187/virtual-serial-port-for-linux

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
//  showMaximized();

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
  /*
   * ;view mode: extend or preserve pic size
   * ;com port config
   * ;source dir
   * ;button size
   * ;serial port name
   */

  mDirPath = qApp->applicationDirPath()+"/pics";
  mPortName = "abc";

  mSerialPortInfo = QSerialPortInfo(mPortName);

  return true;
}

bool MainWindow::CacheExistingFileNames()
{
  return true;
}

bool MainWindow::Init()
{
  mDirWatcher->addPath(mDirPath);

  QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
  if (availablePorts.contains())

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

  QDir d(mDirPath);
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
