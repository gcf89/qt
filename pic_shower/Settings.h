#pragma once

#include <QSerialPortInfo>
#include <QString>

class Settings
{
public:
  bool      showMaximized;
  bool      imageShowFullSize;
  QString   imageDir;
  int       buttonWidth;
  int       buttonHeight;
  QString   portName;
  int       baudRate;
  QString   sendOnAccept;
  QString   sendOnReject;

public:
  Settings();

  void ReadConfig();
  bool IsValid() const;
};
