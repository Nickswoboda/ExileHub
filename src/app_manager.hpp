#pragma once

#include <QObject>
#include <QVector>

#include "app.hpp"

class AppManager : public QObject
{
  Q_OBJECT
public:
  explicit AppManager(QObject* parent = nullptr);
  ~AppManager();

  App& AddApp(const QString& path);

  App* AppAtIndex(int index);

  void RemoveApp(int index);

private:
  void Load();
  void Save();

  // TODO: define Copy Ctor for App so I can create on stack
  QVector<App*> apps_;
};
