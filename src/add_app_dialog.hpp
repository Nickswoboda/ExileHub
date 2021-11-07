#pragma once

#include <QDialog>

#include "api_handler.hpp"
#include "app.hpp"

namespace Ui
{
class AddAppDialog;
}

class AddAppDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AddAppDialog(App* app = nullptr, QWidget* parent = nullptr);
  ~AddAppDialog();

  bool RequiresDownload() const;
  QString Path() const;
  QString Name() const;
  bool AutoUpdate() const;
  bool AutoStart() const;
  bool Detach() const;

  Repository repo_;
  RepoRelease repo_release_;

public slots:
  void accept() override;

private:
  Ui::AddAppDialog* ui;
  App* app_ = nullptr;

  bool InputIsValid();
};
