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

public:
  Settings();

  void ReadConfig();
  bool IsValid() const;
};
