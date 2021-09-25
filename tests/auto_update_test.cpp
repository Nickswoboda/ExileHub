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
  void TestTempFolderDeleted()
  {
    AutoUpdater auto_updater;

    QVERIFY(!QDir("temp").exists());
  }

  void TestNewReleaseAvailable()
  {
    AutoUpdater auto_updater;
    QSignalSpy spy(&auto_updater, SIGNAL(UpdateAvailable(int)));
    auto_updater.CheckForNewRelease("v1.0.0");
    spy.wait();
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    asset_id_ = arguments.at(0).toInt();
  }

  void TestNoNewReleaseAvailable()
  {
    AutoUpdater auto_updater;
    QSignalSpy spy(&auto_updater, SIGNAL(UpdateAvailable(int)));
    auto_updater.CheckForNewRelease("v2.0.0");
    spy.wait();
    QCOMPARE(spy.count(), 0);
  }

  void TestDownloadingRelease()
  {
    AutoUpdater auto_updater;
    auto_updater.DownloadRelease(asset_id_);

    QSignalSpy spy(
        &auto_updater,
        SIGNAL(UpdateComplete(const QString&, const QStringList&, bool)));
    spy.wait();

    QCOMPARE(spy.count(), 1);

    QVERIFY(QDir("temp").exists());
    QVERIFY(QFile::exists("temp/ExileHub.zip"));

    auto current_name =
        QFileInfo(QCoreApplication::applicationFilePath()).fileName();
#ifdef Q_OS_WIN32
    QVERIFY(QFile::exists("temp/" + current_name + ".old"));
#endif

    QVERIFY(QFile::exists(current_name));
  }
};

QTEST_MAIN(AutoUpdateTest)
#include "auto_update_test.moc"
