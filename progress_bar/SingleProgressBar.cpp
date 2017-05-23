#include "SingleProgressBar.h"
#include "ui_SingleProgressBar.h"

QProgressBar *SingleProgressBar::ProgressBar() const
{
  return ui->progressBar;
}

void SingleProgressBar::SetLable(QString text)
{
  ui->label->setText(text);
}


SingleProgressBar::SingleProgressBar(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SingleProgressBar)
{
  ui->setupUi(this);
}

SingleProgressBar::~SingleProgressBar()
{
  delete ui;
}
