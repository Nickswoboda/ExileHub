#include "api_handler.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

QNetworkAccessManager ApiHandler::network_manager_;

void ApiHandler::GetLatestRelease(const Repository& repo,
                                  const std::function<void(QString)> callback)
{
  QUrl app_releases_url(
      "https://api.github.com/repos/nickswoboda/ExileHub/releases/latest");
  QNetworkRequest request(app_releases_url);
  QNetworkReply* reply = network_manager_.get(request);
  QObject::connect(reply, &QNetworkReply::finished, [reply, callback]() {
    OnGetLatestReleaseFinished(reply->readAll(), callback);
  });
}

void ApiHandler::OnGetLatestReleaseFinished(
    const QByteArray& data, const std::function<void(QString)> callback)
{
  auto json_doc = QJsonDocument::fromJson(data);
  auto json_obj = json_doc.object();
  if (!json_obj.contains("name")) {
    qDebug() << "No releases found.";
    callback(QString("Hi"));
    return;
  }

  callback(json_obj["name"].toString());
}
