#pragma once

#include <QWidget>

namespace Ui {
class WidgetSplash;
}

class WidgetSplash : public QWidget
{
  Q_OBJECT

private:
  void PrepareGui();

public:
  explicit WidgetSplash(QWidget *parent = 0);
  ~WidgetSplash();

private:
  Ui::WidgetSplash *ui;
};
