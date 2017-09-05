#pragma once

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>

#include "Settings.h"

QT_USE_NAMESPACE


class SerialPort : public QObject
{
  QSerialPort     mSerialPort;
  QByteArray      mWriteData;
  qint64          mBytesWritten;
  QTimer          mTimer;

  Q_OBJECT

public:
  explicit SerialPort(QObject *parent = nullptr);
  ~SerialPort();

  bool Init(const Settings& settings);
  QString ErrorString() const;
  void Write(const QByteArray &writeData);

private slots:
  void HandleBytesWritten(qint64 bytes);
  void HandleTimeout();
  void HandleError(QSerialPort::SerialPortError error);

signals:
  void WriteFinished(bool ok, QString msg = "");
};
