#include "SerialPort.h"

#include <QCoreApplication>
#include <QDebug>

const int kWriteTimeout = 3000;

QT_USE_NAMESPACE

SerialPort::SerialPort(QObject *parent)
  : QObject(parent)
  , mBytesWritten(0)
{
  mTimer.setSingleShot(true);
  connect(&mSerialPort, &QSerialPort::bytesWritten,
          this, &SerialPort::HandleBytesWritten);
  connect(&mSerialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
          this, &SerialPort::HandleError);
  connect(&mTimer, &QTimer::timeout,
          this, &SerialPort::HandleTimeout);
}

SerialPort::~SerialPort()
{
  mSerialPort.close();
}

bool SerialPort::Init(const Settings &settings)
{
  mSerialPort.setPortName(settings.portName);
  mSerialPort.setBaudRate(settings.baudRate);
  return mSerialPort.open(QIODevice::WriteOnly);
}

QString SerialPort::ErrorString() const
{
  return mSerialPort.errorString();
}

void SerialPort::HandleBytesWritten(qint64 bytes)
{
  mBytesWritten += bytes;
  if (mBytesWritten == mWriteData.size()) {
    mBytesWritten = 0;
    emit WriteFinished(true);
  }
}

void SerialPort::HandleTimeout()
{
  emit WriteFinished(false, QString("Operation timed out for port: %1")
                     .arg(mSerialPort.errorString()));
}

void SerialPort::HandleError(QSerialPort::SerialPortError serialPortError)
{
  if (serialPortError == QSerialPort::WriteError) {
    emit WriteFinished(false, mSerialPort.errorString());
  }
}

void SerialPort::Write(const QByteArray &writeData)
{
  mWriteData = writeData;

  qint64 bytesWritten = mSerialPort.write(writeData);
  if ( (bytesWritten == -1) || (bytesWritten != mWriteData.size()) ) {
    emit WriteFinished(false, mSerialPort.errorString());
  } else {
    mTimer.start(kWriteTimeout);
  }
}
