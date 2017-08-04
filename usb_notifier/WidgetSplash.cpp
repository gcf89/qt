#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "WidgetSplash.h"
#include "ui_WidgetSplash.h"
#include "Control.h"


void WidgetSplash::PrepareGui()
{
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
#ifdef SPLASH_STAYS_ON_TOP
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
#endif

  auto p = palette();
  p.setColor(backgroundRole(), Qt::black);
  setPalette(p);

  int dw = QApplication::desktop()->screenGeometry().width();
  int dh = QApplication::desktop()->screenGeometry().height();

  int scrCnt = QApplication::desktop()->screenCount();
  int auxD = (scrCnt - 1) * 2;
  int totalW = dw * auxD + dw;
  int startX = -(totalW/2);

  int totalH = dh * auxD + dh;
  int startY = -(totalH/2);

  resize(totalW, totalH);
  move(startX, startY);

  WriteDebug(QString::number(totalW) + " "
      + QString::number(totalH) + " "
      + QString::number(startX) + " "
      + QString::number(startY));
}


WidgetSplash::WidgetSplash(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::WidgetSplash)
{
  ui->setupUi(this);
  PrepareGui();
}

WidgetSplash::~WidgetSplash()
{
  delete ui;
}
