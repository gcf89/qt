#include <QFileSystemWatcher>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QPixmap>

#include "MainWindow.h"
#include "ui_MainWindow.h"

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
  mDirPath = qApp->applicationDirPath()+"/pics";
  return true;
}

bool MainWindow::CacheExistingFileNames()
{
  return true;
}

bool MainWindow::Init()
{
  mDirWatcher->addPath(mDirPath);
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
