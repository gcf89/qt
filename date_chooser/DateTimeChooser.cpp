#include "DateTimeChooser.h"
#include "ui_DateTimeChooser.h"

DateTimeChooser::DateTimeChooser(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DateTimeChooser)
{
  ui->setupUi(this);
}

DateTimeChooser::~DateTimeChooser()
{
  delete ui;
}
