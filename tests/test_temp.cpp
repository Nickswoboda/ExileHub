#include <QtTest/QtTest>

#include "main_window.hpp"

class TestTemp : public QObject
{
  Q_OBJECT

private slots:
  void Test()
  {
    MainWindow w;
    w.show();
    QVERIFY(!w.isHidden());
  }
};

QTEST_MAIN(TestTemp)
#include "test_temp.moc"
