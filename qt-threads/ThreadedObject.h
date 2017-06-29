#ifndef THREADEDOBJECT_H
#define THREADEDOBJECT_H

#include <QObject>


class ThreadedObject : public QObject
{
  Q_OBJECT
public:
  ThreadedObject(QObject* parent = nullptr) : QObject(parent) {}

signals:
  finished();

public slots:
  virtual void start() = 0;
};

#endif // THREADEDOBJECT_H
