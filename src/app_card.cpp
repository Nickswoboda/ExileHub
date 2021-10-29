#include "app_card.hpp"
#include "ui_app_card.h"

#include <QAction>

AppCard::AppCard(App& app, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppCard),
    app_(app)
{
    ui->setupUi(this);
    ui->name_label->setText(app_.name_);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    addActions({ui->action_update, ui->action_edit, ui->action_remove});
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


void AppCard::on_action_edit_triggered()
{
    app_.Run();
}


void AppCard::on_action_update_triggered()
{
    app_.Run();
}


void AppCard::on_action_remove_triggered()
{
    emit RemoveAppRequested();
}

