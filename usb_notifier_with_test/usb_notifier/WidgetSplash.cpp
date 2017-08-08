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

  int scrCnt = QApplication::desktop()->screenCount();

  if (Gen::Instance().Width != 0 && Gen::Instance().Height != 0) {
    resize(Gen::Instance().Width, Gen::Instance().Height);
    move(Gen::Instance().PosX, Gen::Instance().PosY);
  } else {
    int dw = QApplication::desktop()->screenGeometry().width();
    int dh = QApplication::desktop()->screenGeometry().height();

#ifdef Q_OS_WIN
    int totalW = 0;
    int totalH = 0;
    int posX = 0;
    int posY = 0;

    if (scrCnt < 2) {
      totalW = dw;
      totalH = dh;
    } else {
      int auxD = (scrCnt - 1) * 2;
      totalW = dw * auxD + dw;
      posX = -(totalW/2) - dw/2;
      totalH = dh * auxD + dh;
      posY = -(totalH/2) - dh/2;
    }
    resize(totalW, totalH);
    move(posX, posY);
#endif

#ifdef Q_OS_UNIX
    resize(dw, dh);
    move(0, 0);
#endif
  }

  WriteDebug("[Splash]"
               " scrCnt: " + QString::number(scrCnt)
             + " totalW: " + QString::number(width())
             + " totalH: " + QString::number(height())
             + " posX:   " + QString::number(geometry().x())
             + " poxY:   " + QString::number(geometry().y()));
}


WidgetSplash::WidgetSplash(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::WidgetSplash)
{
  ui->setupUi(this);
}

WidgetSplash::~WidgetSplash()
{
  delete ui;
}
