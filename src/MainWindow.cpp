#include "MainWindow.hpp"

#include <QComboBox>
#include <QGridLayout>
#include <QRadioButton>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

  // clang-format off
  params_ = new Params{{  4.0, 12.0,  8.0},  // stiffness
                       {  1.0,  2.0,  0.5},  // shape
                       {  0.1,  1.9,  1.1},  // peak 
                       {-10.0,  1.0, -4.5}}; // curvature
  // clang-format on

  // setStyleSheet("background-color: rgb(70,70,70);");
  setFixedSize(1'600, 800);

  QWidget *widget = new QWidget(this);
  setCentralWidget(widget);
  QGridLayout *layout = new QGridLayout(widget);

  layout->addItem(
      new QSpacerItem(INT_MIN, 100, QSizePolicy::Minimum, QSizePolicy::Minimum),
      0, 0, 1, 4);

  layout->addWidget(new QComboBox(), 1, 0, 1, 4);

  layout->addItem(
      new QSpacerItem(INT_MIN, 0, QSizePolicy::Minimum, QSizePolicy::Minimum),
      2, 0, 1, 4);

  layout->addWidget(new QRadioButton("Lateral"), 3, 1, 1, 2);
  layout->addWidget(new QRadioButton("Longitudinal"), 3, 2, 1, 2);

  layout->addItem(
      new QSpacerItem(INT_MIN, 0, QSizePolicy::Minimum, QSizePolicy::Minimum),
      4, 0, 1, 4);

  stiffness_slider_ = new LabeledSlider("Stiffness (B)", params_->stiffness.min,
                                        params_->stiffness.max, widget);
  layout->addWidget(stiffness_slider_, 5, 0, 1, 4);

  shape_slider_ = new LabeledSlider("Shape (C)", params_->shape.min,
                                    params_->shape.max, widget);
  layout->addWidget(shape_slider_, 6, 0, 1, 4);

  peak_slider_ = new LabeledSlider("Peak (D)", params_->peak.min,
                                   params_->peak.max, widget);
  layout->addWidget(peak_slider_, 7, 0, 1, 4);

  curvature_slider_ = new LabeledSlider("Curvature (E)", params_->curvature.min,
                                        params_->curvature.max, widget);
  layout->addWidget(curvature_slider_, 8, 0, 1, 4);

  layout->addItem(
      new QSpacerItem(INT_MIN, 0, QSizePolicy::Minimum, QSizePolicy::Minimum),
      9, 0, 1, 4);

  plot_ = new QwtPlot();
  QwtText title("Magic Formula Visualization");
  title.setColor(Qt::gray);

  plot_->setTitle(title);
  plot_->setAxisScale(QwtPlot::xBottom, -5, 5);
  plot_->setAxisScale(QwtPlot::yLeft, -5000, 5000);

  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->attach(plot_);

  curve_ = new QwtPlotCurve();
  // curve_->setPen(Qt::green, 4);
  curve_->setPen(Qt::red, 4);
  curve_->attach(plot_);

  layout->addWidget(plot_, 0, 4, 10, 4);

  widget->setLayout(layout);
  connect(stiffness_slider_, &LabeledSlider::valueChanged, this,
          &MainWindow::stiffnessChanged);
  connect(shape_slider_, &LabeledSlider::valueChanged, this,
          &MainWindow::shapeChanged);
  connect(peak_slider_, &LabeledSlider::valueChanged, this,
          &MainWindow::peakChanged);
  connect(curvature_slider_, &LabeledSlider::valueChanged, this,
          &MainWindow::curvatureChanged);
  connect(this, &MainWindow::paramsChanged, this, &MainWindow::updatePlot);

  updatePlot();
}

void MainWindow::stiffnessChanged(int val) {
  double min = params_->stiffness.min;
  double max = params_->stiffness.max;
  params_->stiffness.val = min + (max - min) * (val / 100.0);
  emit paramsChanged();
}

void MainWindow::shapeChanged(int val) {
  double min = params_->shape.min;
  double max = params_->shape.max;
  params_->shape.val = min + (max - min) * (val / 100.0);
  emit paramsChanged();
}

void MainWindow::peakChanged(int val) {
  double min = params_->peak.min;
  double max = params_->peak.max;
  params_->peak.val = min + (max - min) * (val / 100.0);
  emit paramsChanged();
}

void MainWindow::curvatureChanged(int val) {
  double min = params_->curvature.min;
  double max = params_->curvature.max;
  params_->curvature.val = min + (max - min) * (val / 100.0);
  emit paramsChanged();
}

void MainWindow::updatePlot() {
  const int points = 1000;
  const double slip_min = -5.0;
  const double slip_max = 5.0;
  QVector<double> xData(points), yData(points);
  const double B = params_->stiffness.val;
  const double C = params_->shape.val;
  const double D = params_->peak.val;
  const double E = params_->curvature.val;
  const double Fz = 3000;

  for (int i = 0; i < points; i++) {
    const double slip = slip_min + (slip_max - slip_min) * (i / (double)points);
    xData[i] = slip;
    const double B_slip = B * slip;
    const double tmp = E * (B_slip - std::atan(B_slip));
    const double tmp2 = std::atan(B_slip - tmp);
    const double tmp3 = std::sin(C * tmp2);
    yData[i] = Fz * D * std::sin(tmp3);
  }

  curve_->setSamples(xData, yData);
  plot_->replot();
}
