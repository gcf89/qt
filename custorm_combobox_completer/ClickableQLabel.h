#pragma once

#include <QLabel>
#include <QMouseEvent>

class ClickableQLabel : public QLabel
{
  Q_OBJECT

public:
  ClickableQLabel(QWidget* parent = 0);

protected:
  void mouseReleaseEvent(QMouseEvent *event);

signals:
  void MouseReleased(QMouseEvent *event);
};
