#include "options_dialog.hpp"

#include <QSettings>

#include "ui_options_dialog.h"

OptionsDialog::OptionsDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::OptionsDialog)
{
  ui->setupUi(this);
  setWindowTitle("Options");

  connect(ui->buttonBox, SIGNAL(accepted()), this,
          SLOT(OnAcceptButtonClicked()));
  connect(ui->check_updates_button, SIGNAL(clicked(bool)), parent,
          SIGNAL(UpdateCheckRequested()));

  QSettings settings;
  bool auto_update = settings.value("options/auto_update", false).toBool();
  if (auto_update) {
    ui->auto_update_checkbox->setCheckState(Qt::CheckState::Checked);
  }
  bool minimize_to_tray =
      settings.value("options/minimize_to_tray", false).toBool();
  if (minimize_to_tray) {
    ui->minimize_to_tray_checkbox->setCheckState(Qt::CheckState::Checked);
  }
}

OptionsDialog::~OptionsDialog()
{
  delete ui;
}

void OptionsDialog::OnAcceptButtonClicked()
{
  QSettings settings;

  bool auto_update =
      ui->auto_update_checkbox->checkState() == Qt::CheckState::Checked;
  bool minimize_to_tray =
      ui->minimize_to_tray_checkbox->checkState() == Qt::CheckState::Checked;

  settings.setValue("options/auto_update", auto_update);
  settings.setValue("options/minimize_to_tray", minimize_to_tray);
}
