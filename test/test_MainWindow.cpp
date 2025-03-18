#include "src/MainWindow.hpp"
#include <QtTest/QtTest>

class TestMainWindow : public QObject {
  Q_OBJECT

private slots:
  void testSliderUpdatesLabel_data();
  void testSliderUpdatesLabel();
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

  QLabel *label = window.findChild<QLabel *>(slider_name + "_display_label");
  QVERIFY(label);
  auto text_init = label->text();

  // simulate a slider value change
  slider->setValue(100);

  QVERIFY(text_init != label->text());
}

QTEST_MAIN(TestMainWindow)
#include "test_MainWindow.moc"
