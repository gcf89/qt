#include "Duration.h"

Duration::Duration()
{

}

// void Duration::SetMax(int _Year, int _Month, int _Day, int _Hour, int _Minutes, int _Seconds)
// {

// }

QString Duration::Get(const QDateTime &_Start, const QDateTime &_End)
{
  if (_Start.isNull() || _End.isNull() || _Start > _End)
    return "";

  qint64 diff = _Start.secsTo(_End);
  qint64 secs = diff % 60;
  diff /= 60;
  qint64 minutes = 0;
  qint64 hours = 0;
  qint64 days = 0;
  qint64 years = 0;

  if (diff != 0) {
    minutes = diff % 60;
    diff /= 60;
    if (diff != 0) {
      hours = diff % 24;
      diff /= 24;
      if (diff != 0) {
        days = diff % 365;
        diff /= 365;
        if (diff != 0) {
          years = diff;
        }
      }
    }
  }

  QString r = "";
  if (years) {
    r = r.append("%0y").arg(QString::number(years));
  }
  if (days || years) {
    r = r.append(" %0d").arg(QString::number(days));
  }
  if (hours || days || years) {
    r = r.append(" %0h").arg(QString::number(hours));
  }
  if (minutes || hours || days || years) {
    r = r.append(" %0m").arg(QString::number(minutes));
  }
  if (secs || minutes || hours || days || years) {
    r = r.append(" %0s").arg(QString::number(secs));
  }

  return r.simplified();
}
