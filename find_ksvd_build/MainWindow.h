#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Ui::MainWindow*       ui;
  QString               mStartPattern;
  QString               mDelimiter;

  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_pushButton_clicked();
  void on_pushButtonOpen_clicked();
};

#endif // MAINWINDOW_H
