#pragma once

#include <QObject>

class AppManager : public QObject
{
  Q_OBJECT
public:
  explicit AppManager(QObject* parent = nullptr);

  bool AddApp(bool from_github, const QString& path);
signals:
};
