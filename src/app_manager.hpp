#pragma once

#include <QObject>

class AppManager : public QObject
{
  Q_OBJECT
public:
  explicit AppManager(QObject* parent = nullptr);

  bool AddApp(bool from_github, QString& path);

private:
  // Returns empty if invalid URL
  QString GetLatestReleaseUrl(QString& path);
signals:
};
