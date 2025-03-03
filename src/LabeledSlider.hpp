#ifndef LABELEDSLIDER_H
#define LABELEDSLIDER_H

#include "RotatedLabel.hpp"
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSlider>
#include <QVBoxLayout>

class LabeledSlider : public QWidget {
  Q_OBJECT
public:
  LabeledSlider(const QString &text, double slider_min, double slider_max,
                QWidget *parent = nullptr);

private:
  QVBoxLayout *main_layout_;
  QHBoxLayout *hlayout_;
  RotatedLabel *label_;
  QSlider *slider_;
  QLCDNumber *lcd_display_;
  double min_{};
  double max_{};

  void updateLCDDisplay(int value);

signals:
  void valueChanged(int value);
};

#endif
