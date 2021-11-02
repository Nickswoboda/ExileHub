#include "app_manager.hpp"

#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

AppManager::AppManager(QObject* parent) : QObject(parent)
{
  Load();
}

AppManager::~AppManager()
{
  Save();
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
  if (index < 0 || index >= apps_.size()) {
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

void AppManager::Load()
{
  QFile file("save.json");
  if (!file.open(QIODevice::ReadOnly)) {
    return;
  }

  auto json_doc = QJsonDocument::fromJson(file.readAll());
  QJsonArray json_arr = json_doc.array();

  for (auto entry : json_arr) {
    QJsonObject json_obj = entry.toObject();

    App& app = AddApp(json_obj["path"].toString());
    app.name_ = json_obj["name"].toString();
    app.repo_.author_ = json_obj["repo author"].toString();
    app.repo_.name_ = json_obj["repo name"].toString();
    app.version_ = json_obj["version"].toString();
    app.auto_check_updates_ = json_obj["auto update"].toBool();
    app.auto_start_ = json_obj["auto start"].toBool();
     app.detach_on_exit_ = json_obj["detach"].toBool();
  }
}

void AppManager::Save()
{
  QFile file("save.json");
  if (!file.open(QIODevice::WriteOnly)) {
    return;
  }

  QJsonArray json_arr;
  for (auto* app : apps_) {
    QJsonObject json_obj;
    json_obj["path"] = app->ExecutablePath();
    json_obj["name"] = app->name_;
    json_obj["repo author"] = app->repo_.author_;
    json_obj["repo name"] = app->repo_.name_;
    json_obj["version"] = app->version_;
    json_obj["auto start"] = app->auto_start_;
    json_obj["auto update"] = app->auto_check_updates_;
        json_obj["detach"] = app->detach_on_exit_;
    json_arr.push_back(json_obj);
  }

  QJsonDocument json_doc;
  json_doc.setArray(json_arr);

  file.write(json_doc.toJson());
}
