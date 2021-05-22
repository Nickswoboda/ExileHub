#pragma once

#include <QMenu>
#include <QSystemTrayIcon>

#include "main_window.hpp"

class TrayIcon : public QSystemTrayIcon
{
  Q_OBJECT
public:
  explicit TrayIcon(MainWindow& window);

private:
  QMenu context_menu_;

private slots:
  void OnActivated(QSystemTrayIcon::ActivationReason reason);
  void RestoreWindow();
  void OnWindowMinimized();
};
