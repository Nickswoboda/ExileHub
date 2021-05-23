#include "tray_icon.hpp"

#include <QSignalSpy>
#include <QtTest/QtTest>

class SystemTrayTest : public QObject
{
  Q_OBJECT

public:
  SystemTrayTest() : tray_icon_(window_)
  {
    QGuiApplication::setOrganizationName("ExileHub-test");
    QGuiApplication::setApplicationName("ExileHub-test");

    auto menu = tray_icon_.contextMenu();
    auto actions = menu->actions();

    for (auto* action : actions) {
      if (action->text() == "Restore") {
        restore_action_ = action;
      } else if (action->text() == "Quit") {
        quit_action_ = action;
      }
    }
  }

  MainWindow window_;
  TrayIcon tray_icon_;
  QAction* restore_action_ = nullptr;
  QAction* quit_action_ = nullptr;

private slots:
  void TestMinimizeToSystemTray()
  {
    QSettings settings;

    settings.setValue("options/minimize_to_tray", false);
    window_.show();
    window_.setWindowState(window_.windowState() | Qt::WindowMinimized);
    QVERIFY(window_.isVisible());
    QVERIFY(!tray_icon_.isVisible());

    settings.setValue("options/minimize_to_tray", true);
    window_.showNormal();
    window_.setWindowState(window_.windowState() | Qt::WindowMinimized);
    QVERIFY(!window_.isVisible());
    QVERIFY(tray_icon_.isVisible());
  }

  void TestSystemTrayDoubleClick()
  {
    emit tray_icon_.activated(QSystemTrayIcon::ActivationReason::DoubleClick);
    QVERIFY(window_.isVisible());
    QVERIFY(!tray_icon_.isVisible());

    window_.setWindowState(window_.windowState() | Qt::WindowMinimized);
  }

  void TestSystemTrayRestoreAction()
  {
    QVERIFY(restore_action_ != nullptr);
    emit restore_action_->triggered();

    QVERIFY(window_.isVisible());
    QVERIFY(!tray_icon_.isVisible());
  }

  // Have not yet found a way to test quitting properly
  /*
  void TestSystemTrayQuitAction()
  {
    QVERIFY(tray_icon_.isVisible());
    QSignalSpy spy(qApp, SIGNAL(aboutToQuit()));
    QVERIFY(quit_action_ != nullptr);
    emit quit_action_->triggered();
    QCOMPARE(spy.count(), 1);
  }*/
};

QTEST_MAIN(SystemTrayTest)
#include "tray_icon_test.moc"
