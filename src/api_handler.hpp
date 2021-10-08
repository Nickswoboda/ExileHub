#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QVector>
#include <functional>

struct Repository {
  QString user_;
  QString name_;
};

struct ReleaseAsset {
  QString name_;
  int id_;
};

class ApiHandler
{
public:
  static void GetLatestRelease(
      const Repository& repo,
      const std::function<void(QString, const QVector<ReleaseAsset>&)>
          callback);

private:
  static void OnGetLatestReleaseFinished(
      QNetworkReply* reply,
      const std::function<void(QString, const QVector<ReleaseAsset>&)>
          callback);

  static QNetworkAccessManager network_manager_;
};
