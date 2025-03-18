#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "LabeledSlider.hpp"
#include "Parameters.hpp"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>

struct PlotData {
  QVector<double> x;
  QVector<double> y;
};

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent, const QString& filepath);
  ~MainWindow() {}

signals:
  void paramsChanged();
  void refFileChanged();
  void plotChanged();

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
  PlotData ref_data_;
  PlotData model_data_;

private slots:
  void stiffnessChanged(int val);
  void shapeChanged(int val);
  void peakChanged(int val);
  void curvatureChanged(int val);
  void selectFilepath();
  void updatePlot();
  void loadReferenceData();
  void updateErrorMetric();
  void updateAxisLabels();
};

#endif
