#include "LabeledSlider.hpp"

LabeledSlider::LabeledSlider(const QString &text, const double slider_min,
                             const double slider_max, QWidget *parent)
    : QWidget(parent), min_(slider_min), max_(slider_max) {
  main_layout_ = new QVBoxLayout();
  hlayout_ = new QHBoxLayout();
  hlayout_->setSpacing(5);
  hlayout_->setContentsMargins(0, 0, 0, 0);

  label_ = new RotatedLabel(text);
  label_->setFixedWidth(200);

  slider_ = new QSlider(Qt::Horizontal);
  slider_->setValue(50);
  slider_->setMaximum(100);

  lcd_display_ = new QLCDNumber();
  lcd_display_->setDigitCount(6);
  lcd_display_->setSmallDecimalPoint(true);
  updateLCDDisplay(50);
  lcd_display_->setFixedWidth(150);

  connect(slider_, &QSlider::valueChanged, this,
          &LabeledSlider::updateLCDDisplay);

  hlayout_->addWidget(label_);
  hlayout_->addWidget(slider_);
  hlayout_->addWidget(lcd_display_);

  main_layout_->addLayout(hlayout_);
  main_layout_->setContentsMargins(0, 0, 0, 0);
  setLayout(main_layout_);

  connect(slider_, &QSlider::valueChanged, this, &LabeledSlider::valueChanged);
}

void LabeledSlider::updateLCDDisplay(int value) {
  lcd_display_->display(min_ + (max_ - min_) * (value / 100.0));
}
