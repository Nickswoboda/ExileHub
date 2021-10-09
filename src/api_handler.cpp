#include "api_handler.hpp"

#include <quazip/JlCompress.h>

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

QNetworkAccessManager ApiHandler::network_manager_;

QByteArray GetNetworkReplyData(QNetworkReply* reply)
{
  QByteArray data = reply->readAll();

  reply->deleteLater();
  if (reply->error() || data.isEmpty()) {
    // clear so on finished slot will not also submit an error
    QMessageBox::critical(nullptr, reply->errorString(),
                          "Unable to get network data.\n" + data);
    data.clear();
  }

  return data;
}

void ApiHandler::GetLatestRelease(
    const Repository& repo,
    const std::function<void(QString, const QVector<ReleaseAsset>&)> callback)
{
  QUrl latest_release_url("https://api.github.com/repos/" + repo.user_ + "/" +
                          repo.name_ + "/releases/latest");
  QNetworkRequest request(latest_release_url);
  QNetworkReply* reply = network_manager_.get(request);
  QObject::connect(reply, &QNetworkReply::finished, [reply, callback]() {
    OnGetLatestReleaseFinished(reply, callback);
  });
}

void ApiHandler::DownloadAsset(
    const Repository& repo, int asset_id,
    const std::function<void(const QStringList&)> callback)
{
  // move to init func
  network_manager_.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
  QUrl asset_url("https://api.github.com/repos/" + repo.user_ + "/" +
                 repo.name_ + "/releases/assets/" + QString::number(asset_id));
  QNetworkRequest request(asset_url);
  request.setRawHeader("Accept", "application/octet-stream");
  QNetworkReply* reply = network_manager_.get(request);
  QObject::connect(reply, &QNetworkReply::finished, [reply, callback]() {
    OnDownloadAssetFinished(reply, callback);
  });
}

void ApiHandler::OnGetLatestReleaseFinished(
    QNetworkReply* reply,
    const std::function<void(QString, const QVector<ReleaseAsset>&)> callback)
{
  auto data = GetNetworkReplyData(reply);
  if (data.isEmpty()) {
    return;
  }

  auto json_doc = QJsonDocument::fromJson(data);
  auto json_obj = json_doc.object();
  if (!json_obj.contains("name")) {
    QMessageBox::critical(nullptr, "No Release Found",
                          "Unable to find any release assets.");
  }

  auto assets_arr = json_obj["assets"].toArray();

  QVector<ReleaseAsset> assets;
  for (auto temp : assets_arr) {
    auto asset_obj = temp.toObject();
    ReleaseAsset asset;
    asset.name_ = asset_obj["name"].toString();
    asset.id_ = asset_obj["id"].toInt();
    assets.push_back(asset);
  }
  callback(json_obj["name"].toString(), assets);
}

void ApiHandler::OnDownloadAssetFinished(
    QNetworkReply* reply,
    const std::function<void(const QStringList&)> callback)
{
  auto data = GetNetworkReplyData(reply);
  if (data.isEmpty()) {
    return;
  }

  if (!QDir("apps").exists()) {
    QDir().mkdir("apps");
  }

  QFile zip_file("temp.zip");
  if (zip_file.open(QIODevice::WriteOnly)) {
    zip_file.write(data);
    zip_file.close();
  }

  // need to somehow get repo info
  auto list = JlCompress::extractDir(zip_file.fileName(), "apps");
  if (list.empty()) {
    qDebug() << "Zip extraction failed.";
    return;
  }
  QStringList executables;

  for (auto& file : list) {
    // will be platform specific
    if (file.endsWith(".exe")) {
      executables.push_back(file);
    }
  }

  callback(executables);
}
