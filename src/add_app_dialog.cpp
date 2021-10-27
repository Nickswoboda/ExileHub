#include "add_app_dialog.hpp"

#include "ui_add_app_dialog.h"

AddAppDialog::AddAppDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::AddAppDialog)
{
  ui->setupUi(this);
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
