#include "LabeledSlider.hpp"
#include "qwt_plot.h"
// #include "ui_mainwindow.h" // Auto-generated header
#include <QApplication>
#include <QComboBox>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QMainWindow window;
  window.setFixedSize(1'600, 800);
  QWidget *widget = new QWidget(&window);
  window.setCentralWidget(widget);
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

  LabeledSlider *stiffness_slider = new LabeledSlider("Stiffness (B)", widget);
  layout->addWidget(stiffness_slider, 5, 0, 1, 4);

  LabeledSlider *shape_slider = new LabeledSlider("Shape (C)", widget);
  layout->addWidget(shape_slider, 6, 0, 1, 4);

  LabeledSlider *peak_slider = new LabeledSlider("Peak (D)", widget);
  layout->addWidget(peak_slider, 7, 0, 1, 4);

  LabeledSlider *curvature_slider = new LabeledSlider("Curvature (E)", widget);
  layout->addWidget(curvature_slider, 8, 0, 1, 4);

  layout->addItem(
      new QSpacerItem(INT_MIN, 0, QSizePolicy::Minimum, QSizePolicy::Minimum),
      9, 0, 1, 4);

  QwtPlot *plot = new QwtPlot();
  QwtText title("Magic Formula Visualization");
  title.setColor(Qt::gray);

  plot->setTitle(title);
  layout->addWidget(plot, 0, 4, 10, 4);

  widget->setLayout(layout);
  window.show();

  return app.exec();
}
