#pragma once

#include <QNetworkAccessManager>
#include <QVector>

struct Repository {
  QString author_;
  QString name_;
};

struct ReleaseAsset {
  QString name_;
  int id_;
};

struct RepoRelease {
  QString version_;
  QVector<ReleaseAsset> assets_;
};

class ApiHandler
{
public:
  static void Init();

  static RepoRelease GetLatestRelease(const Repository& repo);
  static QString DownloadAsset(const Repository& repo,
                               const ReleaseAsset& asset);

private:
  static QNetworkAccessManager network_manager_;
};
