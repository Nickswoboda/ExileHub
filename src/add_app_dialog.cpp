#include "add_app_dialog.hpp"

#include <QFileInfo>
#include <QMessageBox>

#include "api_handler.hpp"
#include "ui_add_app_dialog.h"

AddAppDialog::AddAppDialog(App* app, QWidget* parent)
    : QDialog(parent), ui(new Ui::AddAppDialog), app_(app)
{
  ui->setupUi(this);

  if (app_) {
    ui->name_label->setText("Name:");
    ui->name_edit->setText(app->name_);

    ui->github_radio->hide();
    ui->local_radio->hide();
    if (!app_->repo_.author_.isEmpty()) {
      ui->path_label->hide();
      ui->path_edit->hide();
    }
    ui->path_edit->setText(app->ExecutablePath());
    ui->auto_launch_checkbox->setChecked(app->auto_start_);
    ui->detached_checkbox->setChecked(app->detach_on_exit_);
    ui->update_checkbox->setChecked(app->auto_check_updates_);
  }
}

Repository ExtractRepoFromUrl(const QString& path)
{
  int index = path.indexOf("github.com/");
  if (index == -1) {
    return Repository();
  }

  Repository repo;
  int user_index = index + 11;
  int end_user_index = path.indexOf('/', user_index);
  if (end_user_index == -1) {
    return Repository();
  }
  repo.author_ = path.mid(user_index, end_user_index - user_index);

  int name_index = end_user_index + 1;
  int end_name_index = path.indexOf('/', name_index);
  if (end_name_index == -1) {
    end_name_index = path.size();
  }

  repo.name_ = path.mid(name_index, end_name_index - name_index);

  return repo;
}

AddAppDialog::~AddAppDialog()
{
  delete ui;
}

bool AddAppDialog::RequiresDownload() const
{
  return ui->github_radio->isChecked();
}

QString AddAppDialog::Path() const
{
  return ui->path_edit->text();
}

QString AddAppDialog::Name() const
{
  return ui->name_edit->text();
}

bool AddAppDialog::AutoUpdate() const
{
  return ui->update_checkbox->isChecked();
}

bool AddAppDialog::AutoStart() const
{
  return ui->auto_launch_checkbox->isChecked();
}

bool AddAppDialog::Detach() const
{
  return ui->detached_checkbox->isChecked();
}

void AddAppDialog::accept()
{
  if (!InputIsValid()) {
    return;
  }

  if (app_) {
    app_->name_ = ui->name_edit->text();
    app_->SetExecutablePath(ui->path_edit->text());
    app_->detach_on_exit_ = ui->detached_checkbox->isChecked();
    app_->auto_check_updates_ = ui->update_checkbox->isChecked();
    app_->auto_start_ = ui->auto_launch_checkbox->isChecked();
  }

  QDialog::accept();
}

bool AddAppDialog::InputIsValid()
{
  // if local, check that path is executable
  if (ui->local_radio->isChecked()) {
    QFileInfo file_info(Path());
    // for some reason, isExectuble is true if it's a directory
    if (file_info.isDir() || !file_info.isExecutable()) {
      QMessageBox::critical(
          this, "", "The supplied path does not point to an executable file.");
      return false;
    }
  }
  // if github, check if repository is valid, store list of aviable assets
  else {
    repo_ = ExtractRepoFromUrl(Path());

    if (repo_.author_.isEmpty()) {
      QMessageBox::critical(this, "", "GitHub url is not formatted correctly");
      return false;
    }

    repo_release_ = ApiHandler::GetLatestRelease(repo_);

    if (repo_release_.assets_.isEmpty()) {
      QMessageBox::critical(
          this, "", "Unable to find any release assets from this repository");
      return false;
    }
  }
  return true;
}
