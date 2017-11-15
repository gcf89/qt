#include "ClickableQLabel.h"

ClickableQLabel::ClickableQLabel(QWidget* parent)
  : QLabel(parent)
{
}

void ClickableQLabel::mouseReleaseEvent(QMouseEvent* event)
{
  emit MouseReleased(event);
  QLabel::mouseReleaseEvent(event);
}
