#include <QDebug>
#include <QThread>

#include "MyObject.h"

MyObject::MyObject(QObject *parent)
  : ThreadedObject(parent)
{
  qDebug() << "object created:" << (quintptr)thread()->currentThreadId();
}

MyObject::~MyObject()
{
  qDebug() << "~object:" << (quintptr)thread()->currentThreadId();
}

void MyObject::start()
{
  qDebug() << "object start_slot:" << (quintptr)thread()->currentThreadId();
  emit finished();
}
