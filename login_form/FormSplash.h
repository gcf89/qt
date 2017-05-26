#pragma once

#include <QWidget>

namespace Ui {
class FormSplash;
}

class FormSplash : public QWidget
{
  Q_OBJECT

public:
  explicit FormSplash(QWidget *parent = 0);
  ~FormSplash();

private:
  Ui::FormSplash *ui;
};
