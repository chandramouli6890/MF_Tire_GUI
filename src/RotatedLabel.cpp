#include "RotatedLabel.hpp"

void RotatedLabel::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.translate(width() / 2, height() / 2);
  painter.rotate(angle_);
  painter.drawText(-width() / 2, -height() / 2, width(), height(),
                   Qt::AlignCenter, text());
}
