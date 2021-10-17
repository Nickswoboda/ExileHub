#include "app_manager.hpp"

#include <QDebug>
#include <QDir>

AppManager::AppManager(QObject* parent) : QObject(parent) {}

AppManager::~AppManager()
{
  for (auto* app : apps_) {
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

void AppManager::RemoveApp(int index)
{
  auto* app = AppAtIndex(index);

  if (!app->repo_.author_.isEmpty()) {
    QDir folder("apps/" + app->repo_.author_ + '_' + app->repo_.name_);
    qDebug() << folder.exists();
    if (folder.exists()) {
      folder.removeRecursively();
    }
  }

  apps_.remove(index);
  delete app;
}
