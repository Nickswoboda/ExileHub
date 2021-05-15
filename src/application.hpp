#pragma once

#include <QApplication>

#include "auto_updater.hpp"
#include "main_window.hpp"

class Application : public QApplication
{
  Q_OBJECT

public:
  Application(int& argc, char** argv);

private:
  MainWindow main_window_;
  QString version_ = "v1.0.0";
  AutoUpdater auto_updater_;

private slots:
  void OnUpdateAvailable(int asset_id);
  void OnUpdateComplete();
};
