#include "api_handler.hpp"

#include <QDir>
#include <QEventLoop>
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
    QMessageBox::critical(nullptr, reply->errorString(),
                          "Unable to get network data.\n" + data);

    // clear so on finished slot will not also show an error
    data.clear();
  }

  return data;
}

void ApiHandler::Init()
{
  network_manager_.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

RepoRelease ApiHandler::GetLatestRelease(const Repository& repo)
{
  QUrl latest_release_url("https://api.github.com/repos/" + repo.author_ + "/" +
                          repo.name_ + "/releases/latest");
  QNetworkRequest request(latest_release_url);
  QNetworkReply* reply = network_manager_.get(request);

  // used to make network request synchronous
  QEventLoop loop;
  loop.connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  // should add timer just incase
  loop.exec();

  auto data = GetNetworkReplyData(reply);
  if (data.isEmpty()) {
    return RepoRelease();
  }

  auto json_doc = QJsonDocument::fromJson(data);
  auto json_obj = json_doc.object();
  if (!json_obj.contains("name")) {
    QMessageBox::critical(nullptr, "No Release Found",
                          "Unable to find any release assets.");
  }

  RepoRelease release;
  release.version_ = json_obj["name"].toString();

  auto assets_arr = json_obj["assets"].toArray();
  for (auto temp : assets_arr) {
    auto asset_obj = temp.toObject();
    ReleaseAsset asset;
    asset.name_ = asset_obj["name"].toString();
    asset.id_ = asset_obj["id"].toInt();
    release.assets_.push_back(asset);
  }

  return release;
}

QString ApiHandler::DownloadAsset(const Repository& repo,
                                  const ReleaseAsset& asset)
{
  QUrl asset_url("https://api.github.com/repos/" + repo.author_ + "/" +
                 repo.name_ + "/releases/assets/" + QString::number(asset.id_));
  QNetworkRequest request(asset_url);
  request.setRawHeader("Accept", "application/octet-stream");
  QNetworkReply* reply = network_manager_.get(request);

  // used to make network request synchronous
  QEventLoop loop;
  loop.connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  // should add timer just incase
  loop.exec();

  auto data = GetNetworkReplyData(reply);
  if (data.isEmpty()) {
    return QString();
  }

  QDir().mkdir("temp");
  QFile asset_file("temp/" + asset.name_);
  if (asset_file.open(QIODevice::WriteOnly)) {
    auto num_bytes = asset_file.write(data);
    if (num_bytes != data.size()) {
      QMessageBox::critical(nullptr, "Unable to download asset",
                            "Unable to download asset");
      return QString();
    }
  }

  return asset_file.fileName();
}
