#include "app_manager.hpp"

#include <quazip/JlCompress.h>

#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>

AppManager::AppManager(QObject* parent) : QObject(parent)
{
  network_manager_.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

bool AppManager::AddApp(bool from_github, QString& path)
{
  if (from_github) {
    QUrl api_url(GetLatestReleaseUrl(path));
    if (api_url.isEmpty()) {
      return false;
    }

    qDebug() << "Getting latest release";
    QNetworkRequest request(api_url);
    QNetworkReply* reply = network_manager_.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(GetLatestRelease()));
  }
  return true;
}

void AppManager::DownloadAsset(const QString& repo, int id)
{
  qDebug() << "Downloading asset";
  QUrl asset_url = "https://api.github.com/repos/" + repo +
                   "/releases/assets/" + QString::number(id);
  QNetworkRequest request(asset_url);
  request.setRawHeader("Accept", "application/octet-stream");
  QNetworkReply* reply = network_manager_.get(request);
  reply->setProperty("repo", repo);
  connect(reply, SIGNAL(finished()), this, SLOT(AssetDownloadFinished()));
}

QString AppManager::GetLatestReleaseUrl(QString& path)
{
  // assuming url is http(s)://github.com/user/repo
  // prepend 'api.' before github.com and append /repos/ after

  static QString github_url = "github.com";

  path = path.toLower();

  int github_pos = path.indexOf(github_url);
  if (github_pos == -1) {
    return QString();
  }

  // In theory, could validate rest of path form with regex, but if ill-formed,
  // the http request will fail anyways

  path.insert(github_pos + github_url.size(), "/repos");
  path.remove(0, github_pos);
  path.prepend("https://api.");

  if (!path.endsWith('/')) {
    path.append('/');
  }

  path.append("releases/latest");

  return path;
}

void AppManager::GetLatestRelease()
{
  auto reply = dynamic_cast<QNetworkReply*>(sender());
  reply->deleteLater();
  if (reply->error()) {
    qDebug() << reply->errorString();
    qDebug() << reply->readAll();
    return;
  }

  qDebug() << "Latest Release finished";
  auto json_doc = QJsonDocument::fromJson(reply->readAll());
  auto json_obj = json_doc.object();
  if (!json_obj.contains("name")) {
    qDebug() << "No releases found.";
    return;
  }

  QJsonArray assets = json_obj["assets"].toArray();
  if (assets.empty()) {
    qDebug() << "No release assets found.";
    return;
  }

  if (assets.size() == 1) {
    // TODO: remove hardcoded repo
    DownloadAsset("nickswoboda/all-ears-unturned",
                  assets[0].toObject()["id"].toInt());
  }
  // else emit signal with stating multiple assets found
}

void AppManager::AssetDownloadFinished()
{
  qDebug() << "Asset download";
  auto reply = dynamic_cast<QNetworkReply*>(sender());
  reply->deleteLater();
  if (reply->error()) {
    qDebug() << reply->errorString();
    qDebug() << reply->readAll();
    return;
  }

  if (!QDir("apps").exists()) {
    bool success = QDir().mkdir("apps");
    if (!success) qDebug() << "Unable to create apps directory";
  }

  QString repo = reply->property("repo").toString();
  int index = repo.indexOf('/');
  repo.remove(0, index + 1);

  QFile file("apps/" + repo + ".zip");
  if (file.open(QIODevice::WriteOnly)) {
    file.write(reply->readAll());
    file.close();
  }

  auto list = JlCompress::extractDir(file.fileName(), "apps/" + repo);
  if (list.empty()) {
    qDebug() << "Zip extraction failed.";
    return;
  }

  qDebug() << ".zip extracted successfully";
}
