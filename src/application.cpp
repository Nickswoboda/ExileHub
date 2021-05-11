#include "application.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QtNetwork/QNetworkReply>

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
  main_window_.show();

  connect(&network_manager_, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(OnNetworkRequestFinished(QNetworkReply *)));

  QUrl app_releases_url(
      "https://api.github.com/repos/nickswoboda/tundra-game/releases/latest");
  QNetworkRequest request(app_releases_url);
  network_manager_.get(request);
}

void Application::OnNetworkRequestFinished(QNetworkReply *reply)
{
  if (reply->error()) {
    qDebug() << reply->errorString();
    return;
  }

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

  qDebug() << "New update available";
}
