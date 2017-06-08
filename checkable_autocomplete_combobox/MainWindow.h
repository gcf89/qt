#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  QSortFilterProxyModel* mSFModel;
  QSortFilterProxyModel* mSFModel2;

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_lineEdit_textChanged(const QString &_NewText);

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
