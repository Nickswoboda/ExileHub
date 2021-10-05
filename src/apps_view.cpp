#include "apps_view.hpp"

#include <QLabel>

#include "add_app_dialog.hpp"
#include "ui_apps_view.h"

AppsView::AppsView(AppManager& app_manager, QWidget* parent)
    : QWidget(parent), ui(new Ui::AppsView), app_manager_(app_manager)
{
  ui->setupUi(this);

  connect(ui->add_app_button, SIGNAL(clicked(bool)), this,
          SLOT(OnAddAppRequested()));
  connect(ui->app_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
          SLOT(OnAppDoubleClicked(QListWidgetItem*)));
}

AppsView::~AppsView()
{
  delete ui;
}

void AppsView::OnAddAppRequested()
{
  AddAppDialog dialog;
  auto result = dialog.exec();
  if (result != QDialog::Accepted) {
    return;
  }

  auto& app = app_manager_.AddApp(dialog.Path());
  ui->app_list->addItem(app.name_);
  app.run_detached_ = true;
}

void AppsView::OnAppDoubleClicked(QListWidgetItem* item)
{
  int index = ui->app_list->row(item);
  auto* app = app_manager_.AppAtIndex(index);
  if (app != nullptr) {
    if (app->process_.state() == QProcess::ProcessState::Running) {
      app->Stop();
    } else {
      app->Run();
    }
  }
}
