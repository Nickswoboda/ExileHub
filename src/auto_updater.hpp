#pragma once

#include <QString>
#include <QtNetwork/QNetworkAccessManager>

class AutoUpdater : public QObject
{
  Q_OBJECT

public:
  void CheckForNewRelease(const QString& current_version);

signals:

  void UpdateAvailable(int asset_id);
  // Should probably pass struct of application data that includes all of this
  void UpdateComplete(const QString& executable, const QStringList& args,
                      bool detached);

private:
  QNetworkAccessManager network_manager_;

private slots:
  void ExtractAssetZipData(const QString& path);
};
