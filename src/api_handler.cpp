#include "api_handler.hpp"

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
  QUrl app_releases_url(
      "https://api.github.com/repos/nickswoboda/ExileHub/releases/latest");
  QNetworkRequest request(app_releases_url);
  QNetworkReply* reply = network_manager_.get(request);
  QObject::connect(reply, &QNetworkReply::finished, [reply, callback]() {
    OnGetLatestReleaseFinished(reply, callback);
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
