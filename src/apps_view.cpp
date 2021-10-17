#include "apps_view.hpp"

#include <quazip/JlCompress.h>

#include <QAction>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>

#include "add_app_dialog.hpp"
#include "api_handler.hpp"
#include "ui_apps_view.h"

QStringList ExtractAndMoveFile(const QString& file, const QString& dest_path)
{
  auto list = JlCompress::extractDir(file, dest_path);
  if (list.empty()) {
    qDebug() << "Zip extraction failed.";
    return QStringList();
  }

  QStringList executables;

  for (auto& file_name : list) {
    // will be platform specific
    if (file_name.endsWith(".exe")) {
      executables.push_back(file_name);
    }
  }

  return executables;
}

AppsView::AppsView(AppManager& app_manager, QWidget* parent)
    : QWidget(parent), ui(new Ui::AppsView), app_manager_(app_manager)
{
  ui->setupUi(this);

  connect(ui->add_app_button, SIGNAL(clicked(bool)), this,
          SLOT(OnAddAppRequested()));
  connect(ui->app_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
          SLOT(OnAppDoubleClicked(QListWidgetItem*)));

  ui->app_list->setContextMenuPolicy(Qt::ActionsContextMenu);

  auto update_action = new QAction("Update", this);
  auto remove_action = new QAction("Delete", this);
  connect(update_action, SIGNAL(triggered()), this,
          SLOT(OnAppUpdateRequested()));
  connect(remove_action, SIGNAL(triggered()), this,
          SLOT(OnAppRemoveRequested()));
  ui->app_list->addAction(update_action);
  ui->app_list->addAction(remove_action);
}

AppsView::~AppsView()
{
  delete ui;
}

int AppsView::GetSelectedAssetIndex(const RepoRelease& release) const
{
  if (release.assets_.empty()) {
    return -1;
  }

  if (release.assets_.size() == 1) {
    return 0;
  }

  QStringList list;
  std::transform(release.assets_.begin(), release.assets_.end(),
                 std::back_inserter(list),
                 [](ReleaseAsset const& asset) { return asset.name_; });
  auto selection = QInputDialog::getItem(nullptr, "Multiple assets found",
                                         "Choose an asset", list);

  return list.indexOf(selection);
}

QString AppsView::DownloadAndExtractAsset(const Repository& repo,
                                          const ReleaseAsset& asset)
{
  QString path = ApiHandler::DownloadAsset(repo, asset);
  if (path.isEmpty()) {
    return QString();
  }

  QString dest_folder = "apps/" + repo.author_ + "_" + repo.name_;
  auto executables = ExtractAndMoveFile(path, dest_folder);
  if (executables.empty()) {
    QMessageBox::critical(nullptr, "Unable to extract asset file",
                          "Unable to extract asset file");
    return QString();
  }

  QString executable = executables[0];
  if (executables.size() > 1) {
    auto selection = QInputDialog::getItem(nullptr, "Multiple assets found",
                                           "Choose an asset", executables);

    executable = selection;
  }

  return executable;
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

void AppsView::OnAddAppRequested()
{
  AddAppDialog dialog;
  auto result = dialog.exec();
  if (result != QDialog::Accepted) {
    return;
  }

  Repository repo;
  RepoRelease release;

  QString app_path = dialog.Path();
  if (dialog.RequiresDownload()) {
    repo = ExtractRepoFromUrl(dialog.Path());

    release = ApiHandler::GetLatestRelease(repo);

    if (release.assets_.isEmpty()) {
      qDebug() << "No release assets found";
      return;
    }

    int asset_index = GetSelectedAssetIndex(release);

    app_path = DownloadAndExtractAsset(repo, release.assets_[asset_index]);

    if (app_path.isEmpty()) {
      return;
    }
  }

  auto& app = app_manager_.AddApp(app_path);
  ui->app_list->addItem(app.name_);
  app.repo_ = repo;
  app.version_ = release.version_;
  app.detach_on_exit_ = false;
}

void AppsView::OnAppUpdateRequested()
{
  auto items = ui->app_list->selectedItems();
  if (items.empty()) {
    return;
  }
  int index = ui->app_list->row(items[0]);
  auto* app = app_manager_.AppAtIndex(index);
  app->version_ = "";
  RepoRelease release = ApiHandler::GetLatestRelease(app->repo_);

  if (release.version_ == app->version_) {
    QMessageBox::information(nullptr, "", "The app is up to date");
    return;
  }
  // should probably warn about potentially losing settings
  auto result = QMessageBox::question(
      nullptr, "", "A new version is available, would you like to update?\n");
  if (result != QMessageBox::Yes) {
    return;
  }

  app->Stop();

  if (release.assets_.isEmpty()) {
    qDebug() << "No release assets found";
    return;
  }

  int asset_index = GetSelectedAssetIndex(release);

  QString app_path =
      DownloadAndExtractAsset(app->repo_, release.assets_[asset_index]);

  app->executable_path_ = app_path;
  app->version_ = release.version_;
}

void AppsView::OnAppRemoveRequested()
{
  auto items = ui->app_list->selectedItems();
  if (items.empty()) {
    return;
  }
  int index = ui->app_list->row(items[0]);

  app_manager_.RemoveApp(index);
  delete items[0];
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
