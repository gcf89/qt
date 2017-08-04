#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "DialogInfo.h"
#include "ui_DialogInfo.h"
#include "Control.h"


void DialogInfo::ShowInfo(QString message, bool needLock)
{
  ui->textEdit->setText(message);

  // top lable and bg
  if (needLock) {
    ui->labelTop->setText(tr("Комплект нарушен"));

    QPalette p = ui->labelTop->palette();
    p.setColor(backgroundRole(), Qt::black);
    p.setColor(foregroundRole(), Qt::red);
    ui->labelTop->setPalette(p);

    ui->labelPic->setPixmap(mRedPic);
  } else {
    ui->labelTop->setText(tr("Комплект восстановлен"));

    QPalette p = ui->labelTop->palette();
    p.setColor(backgroundRole(), Qt::black);
    p.setColor(foregroundRole(), mColorGreen);
    ui->labelTop->setPalette(p);

    ui->labelPic->setPixmap(mGreenPic);
  }
}

void DialogInfo::PrepareGui()
{
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

  mGreenPic =     QPixmap(":/icons/lock-xxl-green.png");
  mRedPic =       QPixmap(":/icons/lock-xxl.png");
  mColorGreen =   QColor(0, 218, 26); // green like lablePic

  auto p = palette();
  p.setColor(backgroundRole(), Qt::black);
  setPalette(p);

  int dw = QApplication::desktop()->screenGeometry().width();
  int dh = QApplication::desktop()->screenGeometry().height();
  int x = (dw - width()) / 2;
  int y = (dh - height()) / 2;
  move(x, y);

  WriteDebug("Info coord: " + QString::number(x) + " " + QString::number(y));
}



DialogInfo::DialogInfo(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogInfo)
{
  ui->setupUi(this);
  PrepareGui();
}

DialogInfo::~DialogInfo()
{
  delete ui;
}
