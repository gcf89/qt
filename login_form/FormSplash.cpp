#include "FormSplash.h"
#include "ui_FormSplash.h"


FormSplash::FormSplash(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FormSplash)
{
  ui->setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint);
  ui->label->setPixmap(QPixmap(":/icons/icons/Logo.png"));
}

FormSplash::~FormSplash()
{
  delete ui;
}
