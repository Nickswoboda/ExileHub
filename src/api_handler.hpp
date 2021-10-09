#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QVector>
#include <functional>

struct Repository {
  QString author_;
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
      const std::function<void(const QString&, const QVector<ReleaseAsset>&)>
          callback);
  static void DownloadAsset(const Repository& repo, ReleaseAsset asset,
                            const std::function<void(const QString&)> callback);

private:
  static void OnGetLatestReleaseFinished(
      QNetworkReply* reply,
      const std::function<void(const QString&, const QVector<ReleaseAsset>&)>
          callback);
  static void OnDownloadAssetFinished(
      QNetworkReply* reply, const std::function<void(const QString&)> callback);

  static QNetworkAccessManager network_manager_;
};
