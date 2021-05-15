#pragma once

#include <QString>
#include <QtNetwork/QNetworkAccessManager>

class AutoUpdater : public QObject
{
  Q_OBJECT

public:
  AutoUpdater();
  void CheckForNewRelease(const QString& current_version);
  void DownloadRelease(int asset_id);

signals:
  void UpdateAvailable(int asset_id);
  void UpdateComplete();

private:
  QNetworkAccessManager network_manager_;

private slots:
  void CheckLatestRelease();
  void ExtractAssetZipData();
};
