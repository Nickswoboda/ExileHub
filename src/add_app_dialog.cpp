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
