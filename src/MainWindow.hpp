#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "LabeledSlider.hpp"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>

struct Value {
  double min;
  double max;
  double val;
};

struct Params {
  Value stiffness;
  Value shape;
  Value peak;
  Value curvature;
  Value longitudinal_slip;
  double vertical_force{3000.0};
  bool is_lateral{false};
};

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() {}

signals:
  void paramsChanged();
  void refFileChanged();

private:
  LabeledSlider *stiffness_slider_;
  LabeledSlider *shape_slider_;
  LabeledSlider *peak_slider_;
  LabeledSlider *curvature_slider_;
  QPushButton *select_filepath_;
  QLineEdit *filepath_;
  QwtPlot *plot_;
  QwtPlotCurve *curve_;
  QwtPlotCurve *ref_curve_;
  Params *params_;

private slots:
  void stiffnessChanged(int val);
  void shapeChanged(int val);
  void peakChanged(int val);
  void curvatureChanged(int val);
  void selectFilepath();
  void updatePlot();
  void loadReferenceData();
};

#endif
