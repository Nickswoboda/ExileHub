#include "app_manager.hpp"

#include <QDebug>

AppManager::AppManager(QObject* parent) : QObject(parent) {}

App& AppManager::AddApp(const QString& path)
{
  apps_.push_back(App(path));
  return apps_.back();
}
