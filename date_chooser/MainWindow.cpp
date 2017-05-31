#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DateTimeChooser1.h"


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

void MainWindow::on_toolButton_clicked()
{
  auto t = new DateTimeChooser1();
  t->SetCurrentDateTime();
  if (t->exec() == QDialog::Accepted)
  {
    ui->dateTimeEdit->setDateTime(t->GetDateTime());
  }
  delete t;
}
