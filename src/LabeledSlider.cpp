#include "LabeledSlider.hpp"

#define FONT_COLOR "color: rgb(200,200,200)"

LabeledSlider::LabeledSlider(const QString &text, const double slider_min,
                             const double slider_max, QWidget *parent)
    : QWidget(parent), min_(slider_min), max_(slider_max) {
  main_layout_ = new QVBoxLayout();
  hlayout_ = new QHBoxLayout();
  hlayout_->setSpacing(5);
  hlayout_->setContentsMargins(0, 0, 0, 0);

  label_ = new RotatedLabel(text);
  QFont monospace_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  label_->setFont(monospace_font);
  label_->setFixedWidth(300);
  label_->setStyleSheet(FONT_COLOR);

  slider_ = new QSlider(Qt::Horizontal);
  slider_->setValue(50);
  slider_->setMaximum(100);
  slider_->setStyleSheet(R"(
  QSlider::groove:horizontal {
        background: rgb(180,100,0);
        height: 9px;
        border-radius: 3px;
        border: 2px solid rgb(40,40,40);
    }
    QSlider::handle:horizontal {
        background: gray;
        border: 4px solid rgb(40,40,40);
        width: 26px;
        height: 26px;
        border-radius: 4px;
        margin: -9px 0;
    }
)");

  slider_display_ = new QLabel();
  slider_display_->setFont(monospace_font);
  slider_display_->setFixedWidth(90);
  slider_display_->setStyleSheet(FONT_COLOR);
  updateDisplay(50);

  connect(slider_, &QSlider::valueChanged, this, &LabeledSlider::updateDisplay);

  hlayout_->addWidget(label_);
  hlayout_->addWidget(slider_);
  hlayout_->addWidget(slider_display_);

  main_layout_->addLayout(hlayout_);
  main_layout_->setContentsMargins(0, 0, 0, 0);
  setLayout(main_layout_);

  connect(slider_, &QSlider::valueChanged, this, &LabeledSlider::valueChanged);
}

void LabeledSlider::updateDisplay(int value) {
  slider_display_->setText(
      QString::number(min_ + (max_ - min_) * (value / 100.0)));
}
