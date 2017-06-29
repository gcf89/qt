#include <QDebug>

#include "MyThread.h"

MyThread::MyThread(QObject *parent)
  : QThread(parent)
{
  qDebug() << "my thread created:" << (quintptr)currentThreadId();
}

MyThread::~MyThread()
{
  qDebug() << "~my thread:" << (quintptr)currentThreadId();
}

void MyThread::run()
{
  qDebug() << "my thread run:" << (quintptr)currentThreadId();
  slot1();
  func();
//    QThread::run();
    QMetaObject::invokeMethod(this, "slot1");
}

void MyThread::func()
{
  qDebug() << "my thread func:" << (quintptr)currentThreadId();
}

void MyThread::slot1()
{
  qDebug() << "my thread slot1:" << (quintptr)currentThreadId();
}
