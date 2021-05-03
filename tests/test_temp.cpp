#include <QtTest/QtTest>

class TestTemp : public QObject
{
  Q_OBJECT

private slots:
  void Test()
  {
    QVERIFY(1 == 1);
  }
};

QTEST_MAIN(TestTemp)
#include "test_temp.moc"
