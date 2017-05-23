#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void runQProgressBar();
    void runSingleProgressbar();
    void runDoubleProgressBar();

private:
    QString GetPrimaryText(int cur, int max) const;
    QString GetSecondaryText(int cur, int max) const;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
