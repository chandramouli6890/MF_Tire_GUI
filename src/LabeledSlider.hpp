#ifndef LABELEDSLIDER_H
#define LABELEDSLIDER_H

#include "RotatedLabel.hpp"
#include <QFont>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QLabel>
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
  QLabel *slider_display_;
  double min_{};
  double max_{};

  void updateDisplay(int value);

signals:
  void valueChanged(int value);
};

#endif
