#include "LabeledSlider.hpp"

LabeledSlider::LabeledSlider(const QString &text, QWidget *parent)
    : QWidget(parent) {
  main_layout_ = new QVBoxLayout();
  hlayout_ = new QHBoxLayout();
  hlayout_->setSpacing(5);
  hlayout_->setContentsMargins(0, 0, 0, 0);

  label_ = new RotatedLabel(text);
  // label_->setRotation(270);
  label_->setFixedWidth(200);
  // label_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  slider_ = new QSlider(Qt::Horizontal);
  // slider_->setFixedWidth(20);
  // slider_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  lcd_display_ = new QLCDNumber();
  lcd_display_->setDigitCount(6);
  double val = 999.01;
  lcd_display_->setSmallDecimalPoint(true);
  lcd_display_->display(val);
  lcd_display_->setFixedWidth(150);
  // lcd_display_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  hlayout_->addWidget(label_);
  hlayout_->addWidget(slider_);
  hlayout_->addWidget(lcd_display_);

  main_layout_->addLayout(hlayout_);
  main_layout_->setContentsMargins(0, 0, 0, 0);
  setLayout(main_layout_);
}
