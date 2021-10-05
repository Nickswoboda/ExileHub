#include "app_manager.hpp"

#include <QDebug>

AppManager::AppManager(QObject* parent) : QObject(parent) {}

AppManager::~AppManager()
{
  for (auto* app : apps_) {
    app->Stop();
    delete app;
  }
}

App& AppManager::AddApp(const QString& path)
{
  apps_.push_back(new App(path));
  return *apps_.back();
}

App* AppManager::AppAtIndex(int index)
{
  if (index < 0 || index > apps_.size()) {
    return nullptr;
  }

  return apps_[index];
}
