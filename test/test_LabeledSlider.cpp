#include "src/LabeledSlider.hpp"
#include <QtTest/QtTest>

class TestLabeledSlider : public QObject {
  Q_OBJECT

private slots:
  void testSliderUpdatesLabel();
};

void TestLabeledSlider::testSliderUpdatesLabel() {

  QString slider_name = "test_slider";
  LabeledSlider *labeled_slider =
      new LabeledSlider(slider_name, 0, 100, nullptr);
  QSlider *slider =
      labeled_slider->findChild<QSlider *>(slider_name + "_slider");
  QVERIFY(slider);

  QLabel *label =
      labeled_slider->findChild<QLabel *>("test_slider_display_label");
  QVERIFY(label);
  auto text_init = label->text();
  QCOMPARE("50", label->text());

  slider->setValue(100);
  QVERIFY("100" == label->text());

  slider->setValue(22);
  QVERIFY("22" == label->text());
}

QTEST_MAIN(TestLabeledSlider)
#include "test_LabeledSlider.moc"
