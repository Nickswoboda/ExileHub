#pragma once

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QString>
#include <functional>

struct Repository {
  QString user_;
  QString name_;
};

class ApiHandler
{
public:
  static void GetLatestRelease(const Repository& repo,
                               const std::function<void(QString)> callback);

private:
  static void OnGetLatestReleaseFinished(
      const QByteArray& data, const std::function<void(QString)> callback);

  static QNetworkAccessManager network_manager_;
};
