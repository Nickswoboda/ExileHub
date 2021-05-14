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

  QDir("temp").removeRecursively();

  RequestLatestRelease();
}

void Application::RequestLatestRelease()
{
  QUrl app_releases_url(
      "https://api.github.com/repos/nickswoboda/ExileHub/releases/latest");
  QNetworkRequest request(app_releases_url);
  QNetworkReply* reply = network_manager_.get(request);
  connect(reply, SIGNAL(finished()), this, SLOT(CheckLatestRelease()));
}

void Application::RequestAssetZipData(int asset_id)
{
  QUrl asset_url(
      "https://api.github.com/repos/nickswoboda/ExileHub/releases/assets/" +
      QString::number(asset_id));
  QNetworkRequest request(asset_url);
  request.setRawHeader("Accept", "application/octet-stream");
  QNetworkReply* reply = network_manager_.get(request);
  connect(reply, SIGNAL(finished()), this, SLOT(ExtractAssetZipData()));
}

void Application::CheckLatestRelease()
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
  if (version_ == latest_version) {
    qDebug() << "ExileHub is up to date.";
    return;
  }

  QJsonArray assets = json_obj["assets"].toArray();
  if (assets.empty()) {
    qDebug() << "No release assets found.";
    return;
  }

  auto result = QMessageBox::question(
      &main_window_, "Update Available",
      "A new update is available. Would you like to download it?");
  if (result == QMessageBox::Yes) {
    int asset_id = assets[0].toObject()["id"].toInt();
    RequestAssetZipData(asset_id);
  }
}

void Application::ExtractAssetZipData()
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
}
