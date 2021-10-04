#include "app.hpp"

#include <QFileInfo>

App::App(const QString& executable_path) : executable_path_(executable_path)
{
  QFileInfo info(executable_path_);
  name_ = info.baseName();
}

App::App(const QString& name, const QString& executable_path)
    : name_(name), executable_path_(executable_path)
{
}
