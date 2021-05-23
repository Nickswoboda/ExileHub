#pragma once

#include <QApplication>

#include "auto_updater.hpp"
#include "main_window.hpp"
#include "tray_icon.hpp"

class Application : public QApplication
{
  Q_OBJECT

public:
  Application(int& argc, char** argv);

private:
  MainWindow main_window_;
  TrayIcon system_tray_;
  AutoUpdater auto_updater_;

private slots:
  void OnUpdateAvailable(int asset_id);
  void OnUpdateComplete();
  void CheckForNewAppUpdates();
};
