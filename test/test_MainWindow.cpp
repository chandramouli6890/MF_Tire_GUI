#include "src/MainWindow.hpp"
#include <QtTest/QtTest>

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
  void testSliderUpdatesLabel_data();
  void testSliderUpdatesLabel();
  void testLoadReferenceDataEmitsParamsChanged();
  void testLoadReferenceDataChangesXYLabels();
};

void TestMainWindow::testSliderUpdatesLabel_data() {
  QTest::addColumn<QString>("slider_name");
  QTest::newRow("Stiffness Slider") << "Stiffness (B)";
  QTest::newRow("Shape Slider") << "Shape (C)";
  QTest::newRow("Peak Slider") << "Peak (D)";
  QTest::newRow("Curvature Slider") << "Curvature (E)";
}

void TestMainWindow::testSliderUpdatesLabel() {
  QFETCH(QString, slider_name);

  MainWindow window(nullptr, "../../tire_data_longitudinal.yaml");
  window.show();

  QSlider *slider = window.findChild<QSlider *>(slider_name + "_slider");
  QVERIFY(slider);

  QSignalSpy spy(&window, &MainWindow::paramsChanged);
  QVERIFY(spy.isValid());

  QLabel *label = window.findChild<QLabel *>(slider_name + "_display_label");
  QVERIFY(label);
  auto text_init = label->text();

  // simulate a slider value change
  slider->setValue(100);

  QVERIFY(text_init != label->text());

  QVERIFY(spy.count() > 0);
}

void TestMainWindow::testLoadReferenceDataEmitsParamsChanged() {
  MainWindow window(nullptr, "../../tire_data_longitudinal.yaml");
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
  MainWindow window(nullptr, "../../tire_data_longitudinal.yaml");
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
