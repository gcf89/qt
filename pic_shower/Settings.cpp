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


Settings::Settings()
  : showMaximized(true)
  , imageShowFullSize(true)
  , buttonWidth(0)
  , buttonHeight(0)
{
}

void Settings::ReadConfig()
{
  QString configPath = QApplication::applicationDirPath()+QDir::separator()+"config";
  QFile f(configPath);
  if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&f);
    QString line, key, value;
    while (in.atEnd()) {
      line = in.readLine();
      if (line.startsWith("#")) {
        continue;
      }
      key = line.section(kDelimiter,0,0);
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
