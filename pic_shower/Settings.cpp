#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QFile>

#include "Settings.h"

const QString kDelimiter =              "=";
const QString kShowMaximized =          "show_maximized";
const QString kImageShowFullSize =      "image_show_fullsize";
const QString kImageDir =               "image_dir";
const QString kButtonW =                "button_width";
const QString kButtonH =                "button_height";
const QString kSerialPortName =         "serial_port_name";
const QString kSerialPortBaudRate =     "baud_rate";
const QString kSendOnAccept =           "send_on_accept";
const QString kSendOnReject =           "send_on_reject";


Settings::Settings()
  : showMaximized(true)
  , imageShowFullSize(true)
  , buttonWidth(0)
  , buttonHeight(0)
  , baudRate(9600)
{
}

void Settings::ReadConfig()
{
  QString configPath;
#ifdef QT_DEBUG
  configPath = QApplication::applicationDirPath()+QDir::separator()+"config";
#else
  configPath = "/etc/imganalizer/config";
#endif
  QFile f(configPath);
  if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&f);
    QString line, key, value;
    while (!in.atEnd()) {
      line = in.readLine();
      if (line.startsWith("#")) {
        continue;
      }
      key = line.section(kDelimiter,0,0).trimmed();
      value = line.section(kDelimiter,1);
      value = value.mid(0, value.indexOf("#")).trimmed();

      if (key == kShowMaximized) {
        showMaximized = value.toInt() ? true : false;
      } else if (key == kImageShowFullSize) {
        imageShowFullSize = value.toInt() ? true : false;
      } else if (key == kImageDir) {
        imageDir = value;
      } else if (key == kButtonW) {
        buttonWidth = value.toInt();
      } else if (key == kButtonH) {
        buttonHeight = value.toInt();
      } else if (key == kSerialPortName) {
        portName = value;
      } else if (key == kSerialPortName) {
        baudRate = value.toInt();
      } else if (key == kSendOnAccept) {
        sendOnAccept = value;
      } else if (key == kSendOnReject) {
        sendOnReject = value;
      }
    }
  } else {
    qCritical() << "cannot read settings:" << configPath;
  }
}

bool Settings::IsValid() const
{
  return !imageDir.isEmpty() && !portName.isEmpty();
}
