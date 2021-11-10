#include "auto_updater.hpp"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include "api_handler.hpp"
#include "quazip/JlCompress.h"

void AutoUpdater::CheckForNewRelease(const QString& current_version)
{
  Repository repo = {"nickswoboda", "exilehub"};
  RepoRelease release = ApiHandler::GetLatestRelease(repo);

  if (release.version_ == current_version) {
    return;
  }

#ifdef Q_OS_WIN32
  QString file_to_find = "ExileHub_windows.zip";
#else
  QString file_to_find = "ExileHub_linux.zip";
#endif

  int index = -1;
  for (int i = 0; i < release.assets_.size(); ++i) {
    if (release.assets_[i].name_ == file_to_find) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    qDebug() << "Unable to find correct file to auto-update";
    return;
  }

  QMessageBox::question(
      nullptr, "",
      "A new version of ExileHub is available, would you like to download it?");
  QString path = ApiHandler::DownloadAsset(repo, release.assets_[index]);

  ExtractAssetZipData(path);
}

void AutoUpdater::ExtractAssetZipData(const QString& path)
{
  auto list = JlCompress::extractDir(path, "temp");
  if (list.empty()) {
    qDebug() << "Zip extraction failed.";
    return;
  }

#ifdef Q_OS_WIN32
  QString file_to_find = "ExileHub.exe";
#else
  QString file_to_find = "ExileHub";
#endif

  auto executable_it = std::find_if(
      list.begin(), list.end(),
      [&file_to_find](QString s) { return s.endsWith(file_to_find); });

  if (executable_it == list.end()) {
    qDebug() << "Could not find executable file.";
    return;
  }

  auto current_name =
      QFileInfo(QCoreApplication::applicationFilePath()).fileName();

#ifdef Q_OS_WIN32
  bool success = QFile::rename(current_name, "temp/" + current_name + ".old");
#else
  bool success = QFile::remove(current_name);
#endif

  if (!success) {
    qDebug() << "Unable to rename or remove current executable: "
             << current_name;
  }
  QFile::rename(*executable_it, current_name);

  emit UpdateComplete(current_name, {"--auto-updated"}, true);
}
