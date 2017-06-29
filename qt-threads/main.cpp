#include <QCoreApplication>

#include "ThreadedObject.h"
#include "MyObject.h"
#include "MyThread.h"


QThread* createThread(ThreadedObject* obj)
{
  QThread *th = new QThread();
  obj->moveToThread(th);
  QObject::connect(th, SIGNAL(started()), obj, SLOT(start()));
  QObject::connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
  QObject::connect(th, SIGNAL(finished()), obj, SLOT(deleteLater()));
  QObject::connect(obj, SIGNAL(finished()), th, SLOT(quit()));
  return th;
}

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);


//  MyObject obj;
//  auto th = createThread(&obj);
//  th->start();


  MyThread mth;
  mth.start();
  mth.func();
  mth.slot1();
//  mth.quit();



  return a.exec();
}
