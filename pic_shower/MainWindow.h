#pragma once

#include <QMainWindow>
#include <QSet>
#include <QString>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "Common.h"
#include "Settings.h"

DefineClassS(QFileSystemWatcher);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Ui::MainWindow                *ui;
  QFileSystemWatcherS            mDirWatcher;
  QSet<QString>                  mExistingFileNames;
  Settings                       mSettings;
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public:
  bool ReadSettings();
  bool Init();
  bool CacheExistingFileNames();

private slots:
  void on_pushButtonAccept_clicked();
  void on_pushButtonReject_clicked();
  void OnDirChanged(QString path);
};
