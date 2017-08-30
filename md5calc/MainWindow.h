#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  QString mFilePath;
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_pushButtonChooseFile_clicked();
  void on_pushButtonCalcMD5_clicked();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
