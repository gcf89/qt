#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "ThreadedObject.h"

class MyObject : public ThreadedObject
{
  Q_OBJECT
public:
  explicit MyObject(QObject *parent = 0);
  ~MyObject();


public slots:
  void start() override;
};

#endif // MYOBJECT_H
