#pragma once

#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  QString                   mFilePath;
  QString                   mStrTarget;
  QString                   mStrReplacement;

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_pushButtonOpen_clicked();
  void on_pushButtonReplace_clicked();
  void on_pushButtonSave_clicked();

private:
  Ui::MainWindow *ui;
};
