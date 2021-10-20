#include "app.hpp"

#include <windows.h>

#include <QFileInfo>

App::App(const QString& executable_path) : executable_path_(executable_path)
{
  QFileInfo info(executable_path_);
  name_ = info.baseName();
  process_.setProgram(executable_path_);

  process_.setWorkingDirectory(info.absolutePath());
}

App::App(const QString& name, const QString& executable_path)
    : name_(name), executable_path_(executable_path)
{
}

App::~App()
{
  if (detach_on_exit_) {
    process_.Detach();
  }
}

bool App::Run()
{
  if (process_.state() == QProcess::ProcessState::Running) {
    return false;
  }
  return process_.open();
}

bool App::Stop()
{
  process_.kill();
  return process_.state() != QProcess::ProcessState::Running;
}

BOOL CALLBACK windows_show_callback(HWND handle, LPARAM lParam)
{
  QProcess* process = (QProcess*)lParam;

  unsigned long process_id = 0;

  GetWindowThreadProcessId(handle, &process_id);

  if (process_id == process->processId()) {
    if (GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle)) {
      auto success = ShowWindow(handle, SW_NORMAL);
      qDebug() << success;
    }
  }
  return true;
}

BOOL CALLBACK windows_minimize_callback(HWND handle, LPARAM lParam)
{
  QProcess* process = (QProcess*)lParam;

  unsigned long process_id = 0;

  GetWindowThreadProcessId(handle, &process_id);

  if (process_id == process->processId()) {
    if (GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle)) {
      auto success = ShowWindow(handle, SW_MINIMIZE);
      qDebug() << success;
    }
  }
  return true;
}
void App::Show()
{
  if (process_.state() != QProcess::NotRunning) {
    EnumWindows(&windows_show_callback, (LPARAM)&process_);
    qDebug() << "show";
  }
}

void App::Minimize()
{
  if (process_.state() != QProcess::NotRunning) {
    EnumWindows(&windows_minimize_callback, (LPARAM)&process_);
    qDebug() << "minimize";
  }
}

DetachableProcess::DetachableProcess(QObject* parent) : QProcess(parent) {}

void DetachableProcess::Detach()
{
  // QProcess dtor checks to see if the process is running in order to terminate
  // it by changing the process state, the dtor will not kill the process when
  // the QProcess is destroyed
  setProcessState(ProcessState::NotRunning);
}
