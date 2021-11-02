#pragma once

#include <QProcess>
#include <QString>

#include "api_handler.hpp"

class DetachableProcess : public QProcess
{
public:
  DetachableProcess(QObject* parent = nullptr);

  void Detach();
};

class App
{
public:
  explicit App(const QString& executable_path);

  ~App();

  bool Run();
  bool IsRunning() const;
  bool Stop();
  void Show();
  void Minimize();
  void Update();

  void SetExecutablePath(const QString& path);
  QString ExecutablePath() const;

  QString name_;
  Repository repo_;
  QString version_;
  bool auto_check_updates_ = false;
  bool auto_start_ = false;
  // Determines whether or not the app will close when ExileHub closes, or if it
  // will detach into it's own process
  bool detach_on_exit_ = false;

private:
  DetachableProcess process_;
  QString executable_path_;
};
