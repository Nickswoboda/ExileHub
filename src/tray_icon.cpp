#include "tray_icon.hpp"

#include <QApplication>

TrayIcon::TrayIcon(MainWindow& window) : QSystemTrayIcon(&window)
{
  setToolTip(window.windowTitle());
  setIcon(window.windowIcon());

  context_menu_.addAction("Restore", this, SLOT(RestoreWindow()));
  context_menu_.addAction("Quit", qApp, SLOT(quit()));
  setContextMenu(&context_menu_);

  connect(&window, SIGNAL(Minimized()), this, SLOT(OnWindowMinimized()));
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(OnActivated(QSystemTrayIcon::ActivationReason)));
}

void TrayIcon::OnActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::ActivationReason::DoubleClick) {
    RestoreWindow();
  }
}

void TrayIcon::RestoreWindow()
{
  auto window = dynamic_cast<QWidget*>(parent());
  window->showNormal();
  window->activateWindow();
  hide();
}

void TrayIcon::OnWindowMinimized()
{
  show();
}
