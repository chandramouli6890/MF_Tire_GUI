#include "MainWindow.hpp"

#include <iostream>
#include <yaml-cpp/yaml.h>

#include "qwt_symbol.h"
#include <QFileDialog>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

  // clang-format off
  params_ = new Params{{  4.0, 12.0,  8.0}, // stiffness
                       {  1.0,  2.0,  0.5}, // shape
                       {  0.1,  1.9,  1.1}, // peak
                       {-10.0,  1.0, -4.5}, // curvature
                       { -1.0,  1.0},       // longitudinal_slip
                       3000.0};             // vertical_force
  // clang-format on

  // setStyleSheet("background-color: rgb(70,70,70);");
  setFixedSize(1'600, 800);

  QWidget *widget = new QWidget(this);
  setCentralWidget(widget);
  QGridLayout *layout = new QGridLayout(widget);

  layout->addItem(
      new QSpacerItem(INT_MIN, 100, QSizePolicy::Minimum, QSizePolicy::Minimum),
      0, 0, 1, 4);

  select_filepath_ = new QPushButton("Select data");
  filepath_ = new QLineEdit("../tire_data_longitudinal.yaml");
  filepath_->setEnabled(false);

  layout->addWidget(select_filepath_, 1, 0, 1, 1);
  layout->addWidget(filepath_, 1, 1, 1, 3);

  // horizontal spacer
  layout->addItem(
      new QSpacerItem(70, INT_MIN, QSizePolicy::Minimum, QSizePolicy::Minimum),
      0, 4);

  layout->addItem(
      new QSpacerItem(INT_MIN, 0, QSizePolicy::Minimum, QSizePolicy::Minimum),
      2, 0, 1, 4);

  stiffness_slider_ = new LabeledSlider("Stiffness (B)", params_->stiffness.min,
                                        params_->stiffness.max, widget);
  layout->addWidget(stiffness_slider_, 3, 0, 1, 4);

  shape_slider_ = new LabeledSlider("Shape (C)", params_->shape.min,
                                    params_->shape.max, widget);
  layout->addWidget(shape_slider_, 4, 0, 1, 4);

  peak_slider_ = new LabeledSlider("Peak (D)", params_->peak.min,
                                   params_->peak.max, widget);
  layout->addWidget(peak_slider_, 5, 0, 1, 4);

  curvature_slider_ = new LabeledSlider("Curvature (E)", params_->curvature.min,
                                        params_->curvature.max, widget);
  layout->addWidget(curvature_slider_, 6, 0, 1, 4);

  layout->addItem(
      new QSpacerItem(INT_MIN, 0, QSizePolicy::Minimum, QSizePolicy::Minimum),
      7, 0, 1, 4);

  plot_ = new QwtPlot();
  QwtText title("Magic Formula Visualization");
  title.setColor(Qt::gray);

  plot_->setTitle(title);
  plot_->setAxisScale(QwtPlot::xBottom, -1, 1);
  plot_->setAxisScale(QwtPlot::yLeft, -5000, 5000);
  plot_->setAxisTitle(QwtPlot::xBottom, "Longitudinal Slip [-]");
  plot_->setAxisTitle(QwtPlot::yLeft, "Longitudinal Force[N]");

  ref_curve_ = new QwtPlotCurve();
  QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::gray),
                                    QPen(Qt::gray, 2), QSize(6, 6));
  ref_curve_->setSymbol(symbol);
  ref_curve_->setStyle(QwtPlotCurve::NoCurve);
  ref_curve_->attach(plot_);
  loadReferenceData();

  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->attach(plot_);

  curve_ = new QwtPlotCurve();
  curve_->setPen(Qt::red, 4);
  curve_->attach(plot_);
  updatePlot();

  layout->addWidget(plot_, 0, 5, 8, 4);

  widget->setLayout(layout);
  connect(stiffness_slider_, &LabeledSlider::valueChanged, this,
          &MainWindow::stiffnessChanged);
  connect(shape_slider_, &LabeledSlider::valueChanged, this,
          &MainWindow::shapeChanged);
  connect(peak_slider_, &LabeledSlider::valueChanged, this,
          &MainWindow::peakChanged);
  connect(curvature_slider_, &LabeledSlider::valueChanged, this,
          &MainWindow::curvatureChanged);
  connect(select_filepath_, &QPushButton::clicked, this,
          &MainWindow::selectFilepath);
  connect(this, &MainWindow::paramsChanged, this, &MainWindow::updatePlot);
  connect(this, &MainWindow::refFileChanged, this,
          &MainWindow::loadReferenceData);
}

void MainWindow::selectFilepath() {
  QString filepath = QFileDialog::getOpenFileName(this, "Select a File", "",
                                                  "YAML Files (*.yaml)");

  if (!filepath.isEmpty()) {
    filepath_->setText(filepath);
    qDebug() << filepath_->text();
    emit refFileChanged();
  }
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
  const double slip_min = params_->longitudinal_slip.min;
  const double slip_max = params_->longitudinal_slip.max;
  QVector<double> x_data(points), y_data(points);
  const double B = params_->stiffness.val;
  const double C = params_->shape.val;
  const double D = params_->peak.val;
  const double E = params_->curvature.val;
  const double Fz = params_->vertical_force;

  for (int i = 0; i < points; i++) {
    const double slip = slip_min + (slip_max - slip_min) * (i / (double)points);
    x_data[i] = slip;
    const double B_slip = B * slip;
    const double tmp = E * (B_slip - std::atan(B_slip));
    const double tmp2 = std::atan(B_slip - tmp);
    const double tmp3 = std::sin(C * tmp2);
    y_data[i] = Fz * D * std::sin(tmp3);
  }

  curve_->setSamples(x_data, y_data);
  plot_->replot();
}

void MainWindow::loadReferenceData() {
  QVector<double> slip;
  QVector<double> force;
  QString filename = filepath_->text();
  YAML::Node yaml_data = YAML::LoadFile(filename.toStdString());

  auto tire_data = yaml_data["tire_data"][0];
  double vertical_force = tire_data["vertical_force"].as<double>();
  slip = QVector<double>::fromStdVector(
      tire_data["longitudinal_slip_array"].as<std::vector<double>>());
  force = QVector<double>::fromStdVector(
      tire_data["longitudinal_force_array"].as<std::vector<double>>());

  ref_curve_->setSamples(slip, force);
  plot_->replot();
}
