#include "system_tray.hpp"

#include <QApplication>

#include "main_window.hpp"

SystemTray::SystemTray(QObject* parent)
    : QSystemTrayIcon(parent), icon_(":/assets/icon.png")
{
  setToolTip("ExileHub");
  setIcon(icon_);

  context_menu_.addAction("Restore", this, SLOT(RestoreMainWindow()));
  context_menu_.addAction("Quit", qApp, SLOT(quit()));
  setContextMenu(&context_menu_);

  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(OnActivated(QSystemTrayIcon::ActivationReason)));
}

void SystemTray::OnActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::ActivationReason::DoubleClick) {
    RestoreMainWindow();
  }
}

void SystemTray::RestoreMainWindow()
{
  auto window = dynamic_cast<MainWindow*>(parent());
  window->showNormal();
  window->activateWindow();
  hide();
}
