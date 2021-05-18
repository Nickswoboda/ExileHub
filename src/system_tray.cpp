#include "system_tray.hpp"

#include <QApplication>
#include <QMenu>

SystemTray::SystemTray(QObject* parent) : QSystemTrayIcon(parent)
{
  icon_.addFile("C:/dev/ExileHub/assets/icon.png");
  setIcon(icon_);
  context_menu_.addAction("Restore", parent, SLOT(RestoreFromSystemTray()));
  context_menu_.addAction("Quit", qApp, SLOT(quit()));
  setContextMenu(&context_menu_);
  setToolTip("ExileHub");
}
