#pragma once

#include <QWidget>
#include <QProgressBar>

namespace Ui {
class SingleProgressBar;
}

class SingleProgressBar : public QWidget
{
  Q_OBJECT

public:
  QProgressBar* ProgressBar() const;
  void SetLable(QString text);

public:
  explicit SingleProgressBar(QWidget *parent = 0);
  ~SingleProgressBar();

private:
  Ui::SingleProgressBar *ui;
};
