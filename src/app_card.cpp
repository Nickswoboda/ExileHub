#include "app_card.hpp"
#include "ui_app_card.h"

#include <QAction>
#include <QInputDialog>
#include <QDebug>

AppCard::AppCard(App& app, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppCard),
    app_(app)
{
    ui->setupUi(this);
    ui->name_label->setText(app_.name_);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    addActions({ui->action_rename, ui->action_remove, ui->action_auto_start, ui->action_auto_update, ui->action_detach_on_close});

    ui->action_auto_start->setChecked(app.auto_start_);
    ui->action_auto_update->setChecked(app.auto_check_updates_);
    ui->action_detach_on_close->setChecked(app.detach_on_exit_);

    if (!app_.repo_.author_.isEmpty()){
        addAction(ui->action_update);
    } else {
        addAction(ui->action_edit_path);
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


void AppCard::on_action_rename_triggered()
{
   auto name = QInputDialog::getText(this, "", "Enter a new name");

   app_.name_ = name;
   ui->name_label->setText(app_.name_);
}


void AppCard::on_action_edit_path_triggered()
{
    auto path = QInputDialog::getText(this, "", "Enter a new path");

    app_.SetExecutablePath(path);
}


void AppCard::on_action_auto_start_triggered(bool checked)
{
    app_.auto_start_ = checked;
}


void AppCard::on_action_auto_update_triggered(bool checked)
{
    app_.auto_check_updates_ = checked;
}


void AppCard::on_action_detach_on_close_triggered(bool checked)
{
    app_.detach_on_exit_ = checked;
}

