#include "MainWindow.hpp"

#include "qwt_plot_grid.h"
#include "qwt_plot_marker.h"
#include "qwt_symbol.h"
#include <QFileDialog>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent, const QString &filepath)
    : QMainWindow(parent) {

  // clang-format off
  params_ = new Params{{  4.0, 12.0,  8.0},  // stiffness
                       {  1.0,  2.0,  0.5},  // shape
                       {  0.1,  1.9,  1.1},  // peak
                       {-10.0,  1.0, -4.5}}; // curvature
  // clang-format on

  setWindowTitle("Magic-Formula Tire Visualizer");
  setStyleSheet("background-color: rgb(70,70,70);");
  setFixedSize(1'600, 800);

  QWidget *widget = new QWidget(this);
  setCentralWidget(widget);
  QGridLayout *layout = new QGridLayout(widget);

  layout->addItem(
      new QSpacerItem(INT_MIN, 100, QSizePolicy::Minimum, QSizePolicy::Minimum),
      0, 0, 1, 4);

  QFont monospace_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  select_filepath_ = new QPushButton("Select");
  select_filepath_->setFont(monospace_font);
  select_filepath_->setStyleSheet("color: rgb(200,200,200)");
  filepath_ = new QLineEdit(filepath);
  filepath_->setFont(monospace_font);
  filepath_->setEnabled(false);

  layout->addWidget(select_filepath_, 1, 0, 1, 1);
  layout->addWidget(filepath_, 1, 1, 1, 3);

  // horizontal spacer
  layout->addItem(
      new QSpacerItem(30, INT_MIN, QSizePolicy::Minimum, QSizePolicy::Minimum),
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

  plot_->setAxisScale(QwtPlot::xBottom, -1, 1);
  plot_->setAxisScale(QwtPlot::yLeft, -5000, 5000);

  ref_curve_ = new QwtPlotCurve();
  QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::darkGray),
                                    QPen(Qt::darkGray, 2), QSize(6, 6));
  ref_curve_->setSymbol(symbol);
  ref_curve_->setStyle(QwtPlotCurve::NoCurve);
  ref_curve_->attach(plot_);
  loadReferenceData(YAML::LoadFile(filepath.toStdString()));

  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->attach(plot_);

  curve_ = new QwtPlotCurve();
  curve_->setPen(QPen(Qt::green, 6));
  curve_->attach(plot_);
  updatePlot();
  updateErrorMetric();

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
  connect(select_filepath_, &QPushButton::clicked, this, &MainWindow::loadFile);
  connect(this, &MainWindow::paramsChanged, this, &MainWindow::updatePlot);
  connect(this, &MainWindow::refFileChanged, this,
          &MainWindow::loadReferenceData);
  connect(this, &MainWindow::plotChanged, this, &MainWindow::updateErrorMetric);
}

void MainWindow::loadFile() {
  QString filepath = QFileDialog::getOpenFileName(this, "Select a File", "",
                                                  "YAML Files (*.yaml)");

  if (!filepath.isEmpty()) {
    filepath_->setText(filepath);
    YAML::Node data = YAML::LoadFile(filepath.toStdString());
    emit refFileChanged(data);
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
  const int array_size = params_->array_size;
  const double slip_min = params_->slip.min;
  const double slip_max = params_->slip.max;
  const double B = params_->stiffness.val;
  const double C = params_->shape.val;
  const double D = params_->peak.val;
  const double E = params_->curvature.val;
  const double Fz = params_->vertical_force;

  QVector<double> x(array_size);
  QVector<double> y(array_size);

  for (int i = 0; i < array_size; i++) {
    const double slip =
        slip_min + (slip_max - slip_min) * (i / (double)array_size);
    x[i] = slip;
    const double B_slip = B * slip;
    const double tmp = E * (B_slip - std::atan(B_slip));
    const double tmp2 = std::atan(B_slip - tmp);
    const double tmp3 = std::sin(C * tmp2);
    y[i] = Fz * D * std::sin(tmp3);
  }

  curve_->setSamples(x, y);
  model_data_.x = x;
  model_data_.y = y;
  plot_->replot();
  emit plotChanged();
}

void MainWindow::loadReferenceData(YAML::Node data) {
  auto tire_data = data["tire_data"][0];
  ref_data_.x = QVector<double>::fromStdVector(
      tire_data["slip_array"].as<std::vector<double>>());
  ref_data_.y = QVector<double>::fromStdVector(
      tire_data["force_array"].as<std::vector<double>>());

  ref_curve_->setSamples(ref_data_.x, ref_data_.y);
  plot_->replot();

  params_->is_lateral = tire_data["is_lateral"].as<bool>();
  updateAxisLabels();

  params_->array_size = ref_data_.x.size();
  params_->vertical_force = tire_data["vertical_force"].as<double>();
  params_->slip.min = *std::min_element(ref_data_.x.begin(), ref_data_.x.end());
  params_->slip.max = *std::max_element(ref_data_.x.begin(), ref_data_.x.end());
  emit paramsChanged();
}

void MainWindow::updateErrorMetric() {
  // 1. Compute the mean of ref_y
  double mean_ref =
      std::accumulate(ref_data_.y.begin(), ref_data_.y.end(), 0.0) /
      params_->array_size;

  // 2. Compute the sum of squared differences between true and predicted values
  double sum_squared_error = 0.0;
  double sum_squared_deviation = 0.0;

  for (size_t i = 0; i < params_->array_size; ++i) {
    double error = ref_data_.y[i] - model_data_.y[i];
    double deviation = ref_data_.y[i] - mean_ref;
    sum_squared_error += error * error;
    sum_squared_deviation += deviation * deviation;
  }

  // 3. Compute NMSE
  const double error = sum_squared_error / sum_squared_deviation;

  QwtText text(QString("Error (NMSE): %1").arg(error, 0, 'f', 4));
  QFont monospace_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  monospace_font.setBold(false);
  text.setFont(monospace_font);
  plot_->setTitle(text);
}

void MainWindow::updateAxisLabels() {
  std::string label_pre_fix{""};
  if (params_->is_lateral) {
    label_pre_fix = "Lateral ";
  } else {
    label_pre_fix = "Longitudinal ";
  }
  QFont monospace_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  QwtText x_label = QString::fromStdString(label_pre_fix + "Slip");
  monospace_font.setBold(true);
  x_label.setFont(monospace_font);
  x_label.setColor(Qt::gray);
  plot_->setAxisTitle(QwtPlot::xBottom, x_label);
  QwtText y_label = QString::fromStdString(label_pre_fix + "Force [N]");
  y_label.setFont(monospace_font);
  y_label.setColor(Qt::gray);
  plot_->setAxisTitle(QwtPlot::yLeft, y_label);
}
