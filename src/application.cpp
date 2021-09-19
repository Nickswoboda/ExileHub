#include "application.hpp"

    #include <QMessageBox>
#include <QProcess>
    #include <QSettings>

#include "quazip/JlCompress.h"

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv), system_tray_(main_window_)
{
  setOrganizationName("ExileHub");
  setApplicationName("ExileHub");
  setApplicationVersion("v1.0.0");

  main_window_.show();

  connect(&auto_updater_, SIGNAL(UpdateAvailable(int)), this,
          SLOT(OnUpdateAvailable(int)));
  connect(&auto_updater_, SIGNAL(UpdateComplete()), this,
          SLOT(OnUpdateComplete()));
  connect(&main_window_, SIGNAL(UpdateCheckRequested()), this,
          SLOT(CheckForNewAppUpdates()));

  QSettings settings;
  if (settings.value("options/auto_update").toBool()) {
    CheckForNewAppUpdates();
  }
}

void Application::OnUpdateAvailable(int asset_id)
{
  auto result = QMessageBox::question(
      &main_window_, "Update Available",
      "A new update is available. Would you like to download it?");
  if (result == QMessageBox::Yes) {
    auto_updater_.DownloadRelease(asset_id);
  }
}

void Application::OnUpdateComplete()
{
  auto result =
      QMessageBox::question(&main_window_, "Update Complete",
                            "You must restart the application to apply the "
                            "update. Would you like to restart now?");
  if (result == QMessageBox::Yes) {
    bool detached = QProcess::startDetached("ExileHub.exe", {});
    if (detached) {
      quit();
    }
  }
}

void Application::CheckForNewAppUpdates()
{
  auto_updater_.CheckForNewRelease(applicationVersion());
}
