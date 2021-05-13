#include "application.hpp"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QProcess>
#include <QtNetwork/QNetworkReply>

#include "quazip/JlCompress.h"

Application::Application(int& argc, char** argv) : QApplication(argc, argv)
{
  qDebug() << version_;
  main_window_.show();

  if (QDir("temp").exists()) {
    QDir("temp").removeRecursively();
  }

  connect(&network_manager_, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(OnNetworkRequestFinished(QNetworkReply*)));

  QUrl app_releases_url(
      "https://api.github.com/repos/nickswoboda/ExileHub/releases/latest");
  QNetworkRequest request(app_releases_url);
  network_manager_.get(request);
}

void Application::CheckLatestRelease(QNetworkReply* reply)
{
  auto json_doc = QJsonDocument::fromJson(reply->readAll());
  auto json_obj = json_doc.object();

  if (!json_obj.contains("name")) {
    qDebug() << "No releases found.";
  }

  QString latest_version = json_obj["name"].toString();
  if (version_ == latest_version) {
    qDebug() << "ExileHub is up to date.";
    return;
  }

  auto result = QMessageBox::question(
      &main_window_, "Update Available",
      "A new update is available. Would you like to download it?");

  if (result == QMessageBox::No) {
    return;
  }

  QJsonArray assets = json_obj["assets"].toArray();
  if (assets.empty()) {
    qDebug() << "No release assets found";
    return;
  }

  if (assets.size() > 1) {
    qDebug() << "Multiple release assets found.";
    // TODO: figure out solution for multiple assets
  }

  int asset_id = assets[0].toObject()["id"].toInt();
  QUrl asset_url(
      "https://api.github.com/repos/nickswoboda/ExileHub/releases/assets/" +
      QString::number(asset_id));
  QNetworkRequest request(asset_url);
  request.setRawHeader("Accept", "application/octet-stream");
  network_manager_.get(request);

  reply->deleteLater();
}

void Application::OnNetworkRequestFinished(QNetworkReply* reply)
{
  if (reply->error()) {
    qDebug() << reply->errorString();
    qDebug() << reply->readAll();
    return;
  }

  if (reply->url().toString().endsWith("latest")) {
    CheckLatestRelease(reply);
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
    qDebug() << "Zip extraction failed";
    return;
  }

  auto executable_it = std::find_if(
      list.begin(), list.end(), [](QString s) { return s.endsWith(".exe"); });
  if (executable_it == list.end()) {
    qDebug() << "Could not find executable file.";
    return;
  }

  QFile::rename("ExileHub.exe", "temp/old.exe");
  QFile::rename(*executable_it, "ExileHub.exe");

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

  reply->deleteLater();
}
