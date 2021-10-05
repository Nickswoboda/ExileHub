#include "app.hpp"

#include <QFileInfo>

App::App(const QString& executable_path) : executable_path_(executable_path)
{
  QFileInfo info(executable_path_);
  name_ = info.baseName();
  process_.setProgram(executable_path_);
}

App::App(const QString& name, const QString& executable_path)
    : name_(name), executable_path_(executable_path)
{
}

bool App::Run()
{
  if (process_.state() == QProcess::ProcessState::Running) {
    return false;
  }
  bool success = false;
  if (run_detached_) {
    success = process_.startDetached();
  } else {
    success = process_.open();
  }

  return success;
}

bool App::Stop()
{
  process_.kill();
  return process_.state() != QProcess::ProcessState::Running;
}
