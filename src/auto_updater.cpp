#include "auto_updater.hpp"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork/QNetworkReply>

#include "quazip/JlCompress.h"

AutoUpdater::AutoUpdater()
{
  network_manager_.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

void AutoUpdater::CheckForNewRelease(const QString& current_version)
{
  QUrl app_releases_url(
      "https://api.github.com/repos/nickswoboda/ExileHub/releases/latest");
  QNetworkRequest request(app_releases_url);
  QNetworkReply* reply = network_manager_.get(request);
  reply->setProperty("current version", current_version);
  connect(reply, SIGNAL(finished()), this, SLOT(CheckLatestRelease()));
}

void AutoUpdater::DownloadRelease(int asset_id)
{
  QUrl asset_url(
      "https://api.github.com/repos/nickswoboda/ExileHub/releases/assets/" +
      QString::number(asset_id));
  QNetworkRequest request(asset_url);
  request.setRawHeader("Accept", "application/octet-stream");
  QNetworkReply* reply = network_manager_.get(request);
  connect(reply, SIGNAL(finished()), this, SLOT(ExtractAssetZipData()));
}

void AutoUpdater::CheckLatestRelease()
{
  auto reply = dynamic_cast<QNetworkReply*>(sender());
  reply->deleteLater();
  if (reply->error()) {
    qDebug() << reply->errorString();
    qDebug() << reply->readAll();
    return;
  }

  auto json_doc = QJsonDocument::fromJson(reply->readAll());
  auto json_obj = json_doc.object();
  if (!json_obj.contains("name")) {
    qDebug() << "No releases found.";
    return;
  }

  QString latest_version = json_obj["name"].toString();
  if (reply->property("current version") == latest_version) {
    qDebug() << "ExileHub is up to date.";
    return;
  }

  QJsonArray assets = json_obj["assets"].toArray();
  if (assets.empty()) {
    qDebug() << "No release assets found.";
    return;
  }

#ifdef Q_OS_WIN32
  QString file_to_find = "ExileHub_windows.zip";
#else
  QString file_to_find = "ExileHub_linux.zip";
#endif

  for (int i = 0; i < assets.size(); ++i) {
    auto object = assets[i].toObject();
    if (object["name"].toString() == file_to_find) {
      emit UpdateAvailable(object["id"].toInt());
      return;
    }
  }

  qDebug() << "Unable to find asset in repository: " << file_to_find;
}

void AutoUpdater::ExtractAssetZipData()
{
  auto reply = dynamic_cast<QNetworkReply*>(sender());
  reply->deleteLater();
  if (reply->error()) {
    qDebug() << reply->errorString();
    qDebug() << reply->readAll();
    return;
  }

  if (!QDir("temp").exists()) {
    QDir().mkdir("temp");
  }

  QFile file("temp/ExileHub.zip");
  if (file.open(QIODevice::WriteOnly)) {
    file.write(reply->readAll());
    file.close();
  }

  auto list = JlCompress::extractDir(file.fileName(), "temp");
  if (list.empty()) {
    qDebug() << "Zip extraction failed.";
    return;
  }

#ifdef Q_OS_WIN32
  QString file_to_find = QDir::currentPath() + "/temp/ExileHub.exe";
#else
  QString file_to_find = QDir::currentPath() + "/temp/ExileHub";
#endif

  auto executable_it =
      std::find_if(list.begin(), list.end(),
                   [&file_to_find](QString s) { return s == file_to_find; });

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
