#include "app_manager.hpp"

#include <QDebug>

AppManager::AppManager(QObject* parent) : QObject(parent) {}

bool AppManager::AddApp(bool from_github, const QString& path)
{
  qDebug() << from_github;
  qDebug() << path;

  return true;
}
