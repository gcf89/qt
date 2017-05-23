#include <unistd.h>

#include <QProgressBar>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DoubleProgressBar.h"

void MainWindow::run()
{
  int max = 10000;

  auto f = new QProgressBar();
  // hide close button by removing frame (Qt::WindowTitleHint-only not works)
  f->setWindowFlags(Qt::FramelessWindowHint);
  // block input to all windows
  f->setWindowModality(Qt::ApplicationModal);
  f->setMaximum(max);
  f->show();

  for (int i=0; i<max; ++i) {
    f->setValue(i);
    qApp->processEvents();
    usleep(10);
  }

  f->close();
  delete f;
}

void MainWindow::run2()
{
  int max1 = 10;
  int max2 = max1 / 2;

  DoubleProgressBar* f = new DoubleProgressBar();
  f->setFixedSize(220, 120);
  // hide close button by removing frame (Qt::WindowTitleHint-only not works)
  f->setWindowFlags(Qt::FramelessWindowHint);
  // block input to all windows
  f->setWindowModality(Qt::ApplicationModal);
  f->show();

  f->PrimaryProgressBar()->setMaximum(max1);
  f->SecondaryProgressBar()->setMaximum(max2);

  for (int i=0; i<max1; ++i) {
    f->PrimaryProgressBar()->setValue(i);
    f->SetPrimaryLable(GetPrimaryText(i, max1));
    for (int j=0; j<max2; ++j) {
      f->SecondaryProgressBar()->setValue(j);
      f->SetSecondaryLable(GetSecondaryText(j, max2));
      qApp->processEvents();
      usleep(0.5 * 1000 * 1000);
    }
    qApp->processEvents();
  }

  f->close();
  delete f;
}

QString MainWindow::GetPrimaryText(int cur, int max) const
{
  return trUtf8("Всего обработано: %0/%1").arg(cur).arg(max);
}

QString MainWindow::GetSecondaryText(int cur, int max) const
{
  return trUtf8("Всего обработано: %0/%1").arg(cur).arg(max);
}


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}
