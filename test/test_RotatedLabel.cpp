#include "src/RotatedLabel.hpp"
#include <QtTest/QtTest>

class RotatedLabelTest : public QObject {
  Q_OBJECT

private slots:
  void testRotation();
  void testTextSetting();
};

void RotatedLabelTest::testRotation() {
  RotatedLabel label("");
  label.setRotation(45);

  QCOMPARE(label.angle(), 45); // Ensure rotation is set correctly
}

void RotatedLabelTest::testTextSetting() {
  RotatedLabel label("Hi Chan!");
  QCOMPARE(label.text(), QString("Hi Chan!"));
}

// Register test case
QTEST_MAIN(RotatedLabelTest)
#include "test_RotatedLabel.moc"
