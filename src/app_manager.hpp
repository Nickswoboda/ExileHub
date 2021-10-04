#pragma once

#include <QObject>
#include <QVector>

#include "app.hpp"

class AppManager : public QObject
{
  Q_OBJECT
public:
  explicit AppManager(QObject* parent = nullptr);

  App& AddApp(const QString& path);

private:
  QVector<App> apps_;
};
