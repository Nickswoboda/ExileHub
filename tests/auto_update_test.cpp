#include <QDir>
#include <QSettings>
#include <QSignalSpy>
#include <QtTest/QtTest>

#include "application.hpp"

class AutoUpdateTest : public QObject
{
  Q_OBJECT

public:
  AutoUpdateTest() { QDir().mkdir("temp"); }

  int asset_id_ = -1;
private slots:
  void TestNewReleaseAvailable() {}

  void TestNoNewReleaseAvailable() {}

  void TestDownloadingRelease() {}
};

QTEST_MAIN(AutoUpdateTest)
#include "auto_update_test.moc"
