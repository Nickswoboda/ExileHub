#pragma once

#include <QNetworkAccessManager>
#include <QObject>

class AppManager : public QObject
{
  Q_OBJECT
public:
  explicit AppManager(QObject* parent = nullptr);

  bool AddApp(bool from_github, QString& path);

  void DownloadAsset(const QString& repo, int id);

private:
  // Returns empty if invalid URL
  QString GetLatestReleaseUrl(QString& path);

  QNetworkAccessManager network_manager_;
private slots:
  void GetLatestRelease();
  void AssetDownloadFinished();
signals:
};
