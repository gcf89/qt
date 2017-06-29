#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
  Q_OBJECT
public:
  MyThread(QObject *parent = Q_NULLPTR);
  ~MyThread();

  void run() override;

  void func();

public slots:
  void slot1();
};

#endif // MYTHREAD_H
