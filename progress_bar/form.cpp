#include "form.h"
#include "ui_form.h"


QProgressBar*Form::PrimaryProgressBar() const
{
  return ui->progressBarPrimary;
}

QProgressBar*Form::SecondaryProgressBar() const
{
  return ui->progressBarSecondary;
}

Form::Form(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Form)
{
  ui->setupUi(this);
}

Form::~Form()
{
  delete ui;
}
