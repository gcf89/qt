#pragma once


#include <QDateTime>
#include <QString>

class Duration
{
public:
  Duration();

  //void SetMax(int _Year, int _Month, int _Day, int _Hour, int _Minutes, int _Seconds);

  QString Get(const QDateTime& _Start, const QDateTime& _End);
};
