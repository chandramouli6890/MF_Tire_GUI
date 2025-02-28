#ifndef ROTATEDLABEL_H
#define ROTATEDLABEL_H

// #include "ui_mainwindow.h"

#include <QLabel>
#include <QPainter>

class RotatedLabel : public QLabel {
public:
  RotatedLabel(const QString &text, QWidget *parent = nullptr)
      : QLabel(text, parent) {}

  void setRotation(float angle) { angle_ = angle; };
  float angle() { return angle_; };

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  float angle_{0.0};
};

#endif
