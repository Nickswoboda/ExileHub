#include "app_card.hpp"

#include <QAction>
#include <QDebug>
#include <QInputDialog>

#include "add_app_dialog.hpp"
#include "ui_app_card.h"

AppCard::AppCard(App& app, QWidget* parent)
    : QWidget(parent), ui(new Ui::AppCard), app_(app)
{
  ui->setupUi(this);
  ui->name_label->setText(app_.name_);

  setContextMenuPolicy(Qt::ActionsContextMenu);
  addActions({ui->action_remove, ui->action_edit});

  if (!app_.repo_.author_.isEmpty()) {
    addAction(ui->action_update);
  }
}

AppCard::~AppCard()
{
  delete ui;
}

void AppCard::on_start_button_clicked(bool checked)
{
  app_.Run();
}

void AppCard::on_stop_button_clicked(bool checked)
{
  app_.Stop();
}

void AppCard::on_min_button_clicked(bool checked)
{
  app_.Minimize();
}

void AppCard::on_max_button_clicked(bool checked)
{
  app_.Show();
}

void AppCard::on_action_update_triggered()
{
  emit AppUpdateRequested();
}

void AppCard::on_action_remove_triggered()
{
  emit RemoveAppRequested();
}

void AppCard::on_action_edit_triggered()
{
  AddAppDialog dialog(&app_);
  auto result = dialog.exec();
  if (result == QDialog::Accepted) {
    ui->name_label->setText(app_.name_);
  }
}
