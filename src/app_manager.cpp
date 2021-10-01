#include "app_manager.hpp"

#include <QDebug>

AppManager::AppManager(QObject* parent) : QObject(parent) {}

bool AppManager::AddApp(bool from_github, QString& path)
{
  if (from_github) {
    auto api_url = GetLatestReleaseUrl(path);
    qDebug() << api_url;
    // Check to see if github url is valid
  }
  qDebug() << from_github;
  qDebug() << path;

  return true;
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
