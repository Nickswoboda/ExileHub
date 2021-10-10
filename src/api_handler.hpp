#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QVector>

struct Repository {
  QString author_;
  QString name_;
};

// so I can use with QVariant::fromValue
Q_DECLARE_METATYPE(Repository)

struct ReleaseAsset {
  QString name_;
  int id_;
};

class ApiHandler : public QObject
{
  Q_OBJECT
public:
  static ApiHandler& Instance();
  void GetLatestRelease(const Repository& repo);
  void DownloadAsset(const Repository& repo, ReleaseAsset asset);

private:
  ApiHandler();

  QNetworkAccessManager network_manager_;
private slots:
  void OnGetLatestReleaseFinished();
  void OnDownloadAssetFinished();

signals:
  void LatestReleaseFound(const Repository& repo, const QString& release,
                          const QVector<ReleaseAsset>& assets);
  void AssetDownloadComplete(const Repository& repo, const QString& file_path);
};
