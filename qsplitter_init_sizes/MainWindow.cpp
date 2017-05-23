#include <QList>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  int width = 0;
  if (parentWidget())
    width = parentWidget()->width() / 2;
  else
    width = this->width() / 2;

  ui->splitter->setSizes(QList<int>() << width << width);
}

MainWindow::~MainWindow()
{
  delete ui;
}
