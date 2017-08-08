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

  int auxD = 0;
  int totalW = 0;
  int totalH = 0;
  int startX = 0;
  int startY = 0;

  int scrCnt = QApplication::desktop()->screenCount();
  if (scrCnt < 2) {
    totalW = dw;
    totalH = dh;
  } else {
    auxD = (scrCnt - 1) * 2;
    totalW = dw * auxD + dw;
    startX = -(totalW/2) - dw/2;
    totalH = dh * auxD + dh;
    startY = -(totalH/2) - dh/2;
  }

  if (Gen::Instance().Width != 0 && Gen::Instance().Height != 0) {
    resize(Gen::Instance().Width, Gen::Instance().Height);
    move(Gen::Instance().PosX, Gen::Instance().PosY);
  } else {
    resize(totalW, totalH);
    move(startX, startY);
  }

//  int a = 16000;
//  resize(a,a);
//  move(-a/2,-a/2);

  WriteDebug("[Splash]"
             " screen: " + QString::number(scrCnt)
             + " totalW: " + QString::number(totalW)
             + " totalH: " + QString::number(totalH)
             + " startX: " + QString::number(startX)
             + " startY: " + QString::number(startY));
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
