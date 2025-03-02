#ifndef LABELEDSLIDER_H
#define LABELEDSLIDER_H

#include "RotatedLabel.hpp"
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSlider>
#include <QVBoxLayout>

class LabeledSlider : public QWidget {
public:
  LabeledSlider(const QString &text, QWidget *parent = nullptr);

private:
  QVBoxLayout *main_layout_;
  QHBoxLayout *hlayout_;
  RotatedLabel *label_;
  QSlider *slider_;
  QLCDNumber *lcd_display_;
};

#endif
