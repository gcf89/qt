#pragma once

#include <QWidget>
#include <QProgressBar>

namespace Ui {
class DoubleProgressBar;
}

class DoubleProgressBar : public QWidget
{
  Q_OBJECT

public:
  QProgressBar* PrimaryProgressBar() const;
  QProgressBar* SecondaryProgressBar() const;

public:
  explicit DoubleProgressBar(QWidget *parent = 0);
  ~DoubleProgressBar();

private:
  Ui::DoubleProgressBar *ui;
};
