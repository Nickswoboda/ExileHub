#pragma once

#include <QProcess>
#include <QString>
class App
{
public:
  explicit App(const QString& executable_path);
  App(const QString& name, const QString& executable_path);

  bool Run();
  bool Stop();

  QProcess process_;
  QString name_;
  QString executable_path_;
  bool auto_check_updates_ = false;
  bool run_detached_ = false;
};
