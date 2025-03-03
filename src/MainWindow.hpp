#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "LabeledSlider.hpp"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include <QMainWindow>

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
};

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() {}

signals:
  void paramsChanged();

private:
  LabeledSlider *stiffness_slider_;
  LabeledSlider *shape_slider_;
  LabeledSlider *peak_slider_;
  LabeledSlider *curvature_slider_;
  QwtPlot *plot_;
  QwtPlotCurve *curve_;
  Params *params_;

private slots:
  void stiffnessChanged(int val);
  void shapeChanged(int val);
  void peakChanged(int val);
  void curvatureChanged(int val);
  void updatePlot();
};

#endif
