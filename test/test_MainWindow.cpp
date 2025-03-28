#include "src/MainWindow.hpp"
#include <QtTest/QtTest>

const QString filename = "../../data/tire_data_longitudinal.yaml";

const std::string yaml_test_data = R"(
tire_data:
- force_array:
   - 1.0
  is_lateral: true
  slip_array:
   - 1.0
  vertical_force: 400.0
)";

class TestMainWindow : public QObject {
  Q_OBJECT

private slots:
  void testSliderChangeTriggersReplot_data();
  void testSliderChangeTriggersReplot();
  void testLoadReferenceDataEmitsParamsChanged();
  void testLoadReferenceDataChangesXYLabels();
};

void TestMainWindow::testSliderChangeTriggersReplot_data() {
  QTest::addColumn<QString>("slider_name");
  QTest::newRow("Stiffness Slider") << "Stiffness (B)";
  QTest::newRow("Shape Slider") << "Shape (C)";
  QTest::newRow("Peak Slider") << "Peak (D)";
  QTest::newRow("Curvature Slider") << "Curvature (E)";
}

void TestMainWindow::testSliderChangeTriggersReplot() {
  QFETCH(QString, slider_name);

  MainWindow window(nullptr, filename);
  window.show();

  QSlider *slider = window.findChild<QSlider *>(slider_name + "_slider");
  QVERIFY(slider);
  QwtPlot *plot = window.findChild<QwtPlot *>();
  QVERIFY(plot);
  auto title_init = plot->title().text();

  QSignalSpy params_changed_spy(&window, &MainWindow::paramsChanged);
  QVERIFY(params_changed_spy.isValid());

  // simulate a slider value change
  slider->setValue(100);

  QVERIFY(params_changed_spy.count() > 0);
  // Check whether title, which contains error magnitude, has changed.
  // This indirectly verifies if a replot was triggered
  QVERIFY(title_init != plot->title().text());
}

void TestMainWindow::testLoadReferenceDataEmitsParamsChanged() {
  MainWindow window(nullptr, filename);
  window.show();

  QSignalSpy spy(&window, &MainWindow::paramsChanged);
  QVERIFY(spy.isValid());

  YAML::Node node = YAML::Load(yaml_test_data);

  bool success =
      QMetaObject::invokeMethod(&window, "loadReferenceData",
                                Qt::DirectConnection, Q_ARG(YAML::Node, node));
  QVERIFY(success);

  // Verify if the signal was emitted
  QVERIFY(spy.count() > 0);
}

void TestMainWindow::testLoadReferenceDataChangesXYLabels() {
  MainWindow window(nullptr, filename);
  window.show();

  YAML::Node node = YAML::Load(yaml_test_data);

  QwtPlot *plot = window.findChild<QwtPlot *>();
  QVERIFY(plot);

  QString initial_x_label = plot->axisTitle(QwtPlot::xBottom).text();
  QString initial_y_label = plot->axisTitle(QwtPlot::yLeft).text();

  bool success =
      QMetaObject::invokeMethod(&window, "loadReferenceData",
                                Qt::DirectConnection, Q_ARG(YAML::Node, node));
  QVERIFY(success);

  QString final_x_label = plot->axisTitle(QwtPlot::xBottom).text();
  QString final_y_label = plot->axisTitle(QwtPlot::yLeft).text();

  QVERIFY(initial_x_label != final_x_label);
  QVERIFY(initial_y_label != final_y_label);
}

QTEST_MAIN(TestMainWindow)
#include "test_MainWindow.moc"
