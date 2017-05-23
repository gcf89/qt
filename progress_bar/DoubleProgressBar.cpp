#include "DoubleProgressBar.h"
#include "ui_DoubleProgressBar.h"


QProgressBar* DoubleProgressBar::PrimaryProgressBar() const
{
  return ui->progressBarPrimary;
}

QProgressBar* DoubleProgressBar::SecondaryProgressBar() const
{
  return ui->progressBarSecondary;
}

DoubleProgressBar::DoubleProgressBar(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DoubleProgressBar)
{
  ui->setupUi(this);
}

DoubleProgressBar::~DoubleProgressBar()
{
  delete ui;
}
