#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QStyledItemDelegate>

#include "Delegate.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  mModel = new Model(this);
  ui->tableView->setModel(mModel);
  Delegate* d = new Delegate(this);
  ui->tableView->setItemDelegateForColumn(0, d);
}

MainWindow::~MainWindow()
{
  delete ui;
}
