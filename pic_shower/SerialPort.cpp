//#include "SerialPort.h"

//SerialPort::SerialPort()
//{

//}


//#include "threadcom.h"

//WorkComPort::WorkComPort()
//{
//    m_hComPort      = 0;
//    m_stringComPort = "";
//    m_stringMessage = "";
//}

//WorkComPort::~WorkComPort()
//{
//    this->terminate();
//    this->wait(1);
//}

//void WorkComPort::setStringPort(QString port)
//{
//    m_stringComPort = port;
//}

//QString WorkComPort::getStringMessage() const
//{
//    return m_stringMessage;
//}

//void WorkComPort::setComPortSettings(DWORD baud, BYTE stopBits, BYTE byteSize, BYTE parity, DWORD dtr, DWORD rts)
//{
//    m_baudrate  = baud;
//    m_stopbits  = stopBits;
//    m_bytesize  = byteSize;
//    m_parity    = parity;
//    m_dtr       = dtr;
//    m_rts       = rts;
//}

//bool WorkComPort::openComPort()
//{
//    m_mutex.lock();
//    if (m_hComPort != 0)
//    {
//        m_mutex.unlock();
//        return false;
//    }

//    m_hComPort = CreateFileA(m_stringComPort.toAscii(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
//                           0, OPEN_EXISTING, 0, 0);

//    if (m_hComPort == INVALID_HANDLE_VALUE)
//    {
//        m_stringMessage = tr("Port %1 not open.").arg(m_stringComPort);
//        m_mutex.unlock();
//        return false;
//    }
//    else
//    {
//        m_stringMessage = tr("Port %1 open.").arg(m_stringComPort);
//        m_mutex.unlock();
//        return true;
//    }
//}

//bool WorkComPort::isBusyComPort() const
//{
//    return (m_hComPort) ? false : true;
//}

//bool WorkComPort::configComPort()
//{
//    m_mutex.lock();
//    if (!(GetCommState(m_hComPort, &dcb)))
//    {
//        CloseHandle(m_hComPort);
//        m_mutex.unlock();
//        return false;
//    }

//    //------------------
//    dcb.BaudRate = m_baudrate;
//    dcb.ByteSize = m_bytesize;
//    dcb.Parity   = m_parity;
//    dcb.StopBits = m_stopbits;

//    dcb.fDtrControl = m_dtr;
//    dcb.fRtsControl = m_rts;
//    //------------------


//    if (!(SetCommState(m_hComPort, &dcb)))
//    {
//        CloseHandle(m_hComPort);
//        m_mutex.unlock();
//        return false;
//    }

//    comTimeOuts.ReadIntervalTimeout         = 0;
//    comTimeOuts.ReadTotalTimeoutConstant    = 0;
//    comTimeOuts.ReadTotalTimeoutMultiplier  = 0;
//    comTimeOuts.WriteTotalTimeoutConstant   = 0;
//    comTimeOuts.WriteTotalTimeoutMultiplier = 0;

//    SetCommTimeouts(m_hComPort, &comTimeOuts);

//    msleep(500);
//    PurgeComm(m_hComPort, PURGE_RXCLEAR | PURGE_TXCLEAR);
//    msleep(500);

//    SetupComm(m_hComPort, 256, 256);
//    SetCommMask(m_hComPort, EV_BREAK | EV_RXCHAR | EV_TXEMPTY | EV_RXFLAG);

//    m_mutex.unlock();
//    return true;
//}

//void WorkComPort::closeComPort()
//{
//    m_mutex.lock();
//    m_stringMessage = tr("Port %1 close.").arg(m_stringComPort);
//    CloseHandle(m_hComPort);
//    m_mutex.unlock();
//    m_hComPort = 0;
//}

//DWORD WorkComPort::getReceiveBytes()
//{
//    DWORD err = 0;
//    comStat.cbInQue = 0;
//    ClearCommError(m_hComPort, &err, &comStat);
//    return comStat.cbInQue;
//}

//DWORD WorkComPort::readFromComPort(BYTE *inBuff, DWORD nInBytes)
//{
//    DWORD iBytesReaden = 0;
//    ReadFile(m_hComPort, inBuff, nInBytes, &iBytesReaden, 0);
//    return iBytesReaden;
//}

//void WorkComPort::run()
//{
//    unsigned long countBytes = 0;
//    const int SIZE = 2048;
//    unsigned char data[SIZE];

//    while (1)
//    {
//        msleep(1);
//        m_mutex.lock();
//        countBytes = getReceiveBytes();
//        m_mutex.unlock();

//        if (countBytes > 0)
//        {
//            unsigned long readCountBytes = 0;
//            readCountBytes = readFromComPort(data, countBytes);
//            data[readCountBytes] = '\0';

//            arrayFromComPort(data, readCountBytes);
//        }
//    }
//}

//void WorkComPort::arrayFromComPort(const unsigned char *buf, const int size)
//{
//    QByteArray bArr;

//    for (int index = 0; index < size; index++)
//        bArr[index] = buf[index];

//    emit send_data(bArr);
//}

//void WorkComPort::writeToComPort(unsigned char code)
//{
//    DWORD iBytesWritten = 0;
//    DWORD iBytesWrite = 1;
//    BYTE buf[1];

//    buf[0] = code;

//    WriteFile(m_hComPort, buf, iBytesWrite, &iBytesWritten, NULL);
//}

//void WorkComPort::writeToComPort(QString &string)
//{
//    DWORD iBytesWritten = 0;

//    QByteArray data = QByteArray::fromHex(string.toAscii());

//    WriteFile(m_hComPort, data.constData(), data.size(), &iBytesWritten, NULL);
//}
