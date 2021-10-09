#include "apps_view.hpp"

#include <QInputDialog>
#include <QLabel>

#include "add_app_dialog.hpp"
#include "api_handler.hpp"
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
  repo.user_ = path.mid(user_index, end_user_index - user_index);

  int name_index = end_user_index + 1;
  int end_name_index = path.indexOf('/', name_index);
  if (end_name_index == -1) {
    end_name_index = path.size();
  }

  repo.name_ = path.mid(name_index, end_name_index - name_index);

  return repo;
}

void AppsView::OnAddAppRequested()
{
  AddAppDialog dialog;
  auto result = dialog.exec();
  if (result != QDialog::Accepted) {
    return;
  }

  if (dialog.RequiresDownload()) {
    Repository repo = ExtractRepoFromUrl(dialog.Path());
    ApiHandler::GetLatestRelease(repo, [&, temp = repo](
                                           QString latest_release,
                                           QVector<ReleaseAsset> assets) {
      if (latest_release.isEmpty() || assets.isEmpty()) return;

      int asset_id = assets[0].id_;
      if (assets.size() > 1) {
        QStringList list;
        std::transform(assets.begin(), assets.end(), std::back_inserter(list),
                       [](ReleaseAsset const& asset) { return asset.name_; });
        auto selection = QInputDialog::getItem(nullptr, "Multiple assets found",
                                               "Choose an asset", list);

        auto index = list.indexOf(selection);
        asset_id = assets[index].id_;
      }
      ApiHandler::DownloadAsset(
          temp, asset_id, [&](const QStringList& executables) {
            auto& app = app_manager_.AddApp(executables[0]);
            ui->app_list->addItem(app.name_);
          });
    });
    return;
  }
  auto& app = app_manager_.AddApp(dialog.Path());
  ui->app_list->addItem(app.name_);
  app.detach_on_exit_ = true;

  /*
  ApiHandler::GetLatestRelease(
      {"nickswoboda", "ExileHub"},
      [&](QString latest_release, QVector<ReleaseAsset> assets) {
        if (latest_release.isEmpty()) return;

        ApiHandler::DownloadAsset({"nickswoboda", "ExileHub"},assets[1].id_,
  [&](const QStringList& executables){ qDebug() << executables[0]; auto& app =
  app_manager_.AddApp(executables[0]); ui->app_list->addItem(app.name_);
        });
        }
      );
  */
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
