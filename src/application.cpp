#include "application.hpp"

#include <QDir>
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
  connect(&auto_updater_,
          SIGNAL(UpdateComplete(const QString&, const QStringList&, bool)),
          this,
          SLOT(OnUpdateComplete(const QString&, const QStringList&, bool)));
  connect(&main_window_, SIGNAL(UpdateCheckRequested()), this,
          SLOT(CheckForNewAppUpdates()));

  for (int i = 1; i < argc; ++i) {
    if (QString(argv[i]) == "--auto-updated") {
      QDir("temp").removeRecursively();
      QMessageBox::information(&main_window_, "Update Complete",
                               "ExileHub has been successfully updated.");
      return;
    }
  }

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

void Application::OnUpdateComplete(const QString& executable,
                                   const QStringList args, bool detached)
{
  auto result =
      QMessageBox::question(&main_window_, "Update Complete",
                            "You must restart the application to apply the "
                            "update. Would you like to restart now?");
  if (result == QMessageBox::Yes) {
    bool success = false;
    if (detached) {
      success = QProcess::startDetached(executable, args);
    } else {
      success = QProcess::execute(executable, args);
    }

    if (!success) {
      qDebug() << "Unable to open updated executable: " << executable;
    }

    if (executable == "ExileHub" || executable == "ExileHub.exe") {
      qApp->quit();
    }
  }
}

void Application::CheckForNewAppUpdates()
{
  auto_updater_.CheckForNewRelease(applicationVersion());
}
