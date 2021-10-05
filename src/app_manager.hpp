#pragma once

#include <QObject>
#include <vector>

#include "app.hpp"

class AppManager : public QObject
{
  Q_OBJECT
public:
  explicit AppManager(QObject* parent = nullptr);
  ~AppManager();

  App& AddApp(const QString& path);

  App* AppAtIndex(int index);

private:
  // TODO: define Copy Ctor for App so I can create on stack
  std::vector<App*> apps_;
};
