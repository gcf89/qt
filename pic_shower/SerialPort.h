#ifndef SERIALPORT_H
#define SERIALPORT_H


class SerialPort
{
public:
  SerialPort();
};

#endif // SERIALPORT_H



#include <QThread>
#include <QMutex>
#include <QDebug>

#include <windows.h>

class WorkComPort : public QThread
{
    Q_OBJECT
public:
    WorkComPort();
    ~WorkComPort();

    void setStringPort(QString port);
    QString getStringMessage() const;

    bool openComPort();
    bool configComPort();
    bool isBusyComPort() const;
    void closeComPort();
    void setComPortSettings(DWORD baud, BYTE stopBits, BYTE byteSize, BYTE parity, DWORD dtr, DWORD rts);

    void writeToComPort(unsigned char code);
    void writeToComPort(QString &string);

signals:
    void send_data(QByteArray arr);

private:
    HANDLE          m_hComPort;
    DCB             dcb;
    COMMTIMEOUTS    comTimeOuts;
    COMSTAT         comStat;

    QString         m_stringComPort;
    QString         m_stringMessage;

    QMutex          m_mutex;

    DWORD           m_baudrate;
    BYTE            m_stopbits;
    BYTE            m_bytesize;
    BYTE            m_parity;
    DWORD           m_dtr;
    DWORD           m_rts;

    DWORD getReceiveBytes();
    DWORD readFromComPort(BYTE *inBuff, DWORD nInBytes);

    void arrayFromComPort(const unsigned char* buf, const int size);

protected:
    void run();
};
