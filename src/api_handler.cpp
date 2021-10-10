#include "api_handler.hpp"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

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

ApiHandler::ApiHandler()
{
  network_manager_.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

ApiHandler& ApiHandler::Instance()
{
  static ApiHandler api_handler;
  return api_handler;
}

void ApiHandler::GetLatestRelease(const Repository& repo)
{
  QUrl latest_release_url("https://api.github.com/repos/" + repo.author_ + "/" +
                          repo.name_ + "/releases/latest");
  QNetworkRequest request(latest_release_url);
  QNetworkReply* reply = network_manager_.get(request);
  reply->setProperty("repo", QVariant::fromValue(repo));
  QObject::connect(reply, SIGNAL(finished()), this,
                   SLOT(OnGetLatestReleaseFinished()));
}

void ApiHandler::DownloadAsset(const Repository& repo, ReleaseAsset asset)
{
  QUrl asset_url("https://api.github.com/repos/" + repo.author_ + "/" +
                 repo.name_ + "/releases/assets/" + QString::number(asset.id_));
  QNetworkRequest request(asset_url);
  request.setRawHeader("Accept", "application/octet-stream");
  QNetworkReply* reply = network_manager_.get(request);
  reply->setProperty("asset_name", asset.name_);
  reply->setProperty("repo", QVariant::fromValue(repo));
  QObject::connect(reply, SIGNAL(finished()), this,
                   SLOT(OnDownloadAssetFinished()));
}

void ApiHandler::OnGetLatestReleaseFinished()
{
  auto reply = dynamic_cast<QNetworkReply*>(sender());
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
  emit LatestReleaseFound(reply->property("repo").value<Repository>(),
                          json_obj["name"].toString(), assets);
}

void ApiHandler::OnDownloadAssetFinished()
{
  auto reply = dynamic_cast<QNetworkReply*>(sender());
  auto data = GetNetworkReplyData(reply);
  if (data.isEmpty()) {
    return;
  }

  QDir().mkdir("temp");
  QFile asset_file("temp/" + reply->property("asset_name").toString());
  if (asset_file.open(QIODevice::WriteOnly)) {
    auto num_bytes = asset_file.write(data);
    if (num_bytes != data.size()) {
      QMessageBox::critical(nullptr, "Unable to download asset",
                            "Unable to download asset");
      return;
    }
  }

  emit AssetDownloadComplete(reply->property("repo").value<Repository>(),
                             asset_file.fileName());
}
