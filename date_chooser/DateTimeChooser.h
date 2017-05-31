#pragma once

#include <QWidget>

namespace Ui {
class DateTimeChooser;
}

class DateTimeChooser : public QWidget
{
  Ui::DateTimeChooser *ui;

  Q_OBJECT

public:
  explicit DateTimeChooser(QWidget *parent = 0);
  ~DateTimeChooser();
};
