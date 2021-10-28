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
  auto show_action = new QAction("Show", this);
  auto minimize_action = new QAction("Minimize", this);
  connect(update_action, SIGNAL(triggered()), this,
          SLOT(OnAppUpdateRequested()));
  connect(remove_action, SIGNAL(triggered()), this,
          SLOT(OnAppRemoveRequested()));
  connect(show_action, SIGNAL(triggered()), this, SLOT(OnAppShowRequested()));
  connect(minimize_action, SIGNAL(triggered()), this,
          SLOT(OnAppMinimizeRequested()));
  ui->app_list->addActions(
      {update_action, remove_action, show_action, minimize_action});

  int i = 0;
  auto* app = app_manager.AppAtIndex(i);
  while (app) {
    ui->app_list->addItem(app->name_);

    if (app->auto_check_updates_){
        ui->app_list->setCurrentRow(i, QItemSelectionModel::SelectionFlag::Select);
        OnAppUpdateRequested();
        ui->app_list->setCurrentRow(i, QItemSelectionModel::SelectionFlag::Deselect);
    }

    if (app->auto_start_){
        app->Run();
    }

    app = app_manager.AppAtIndex(++i);
  }
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

  if (!dialog.Name().isEmpty()){
      app.name_ = dialog.Name();
  }
  ui->app_list->addItem(app.name_);
  app.repo_ = repo;
  app.version_ = release.version_;
  app.detach_on_exit_ = false;
  app.auto_check_updates_ = dialog.AutoUpdate();
  app.auto_start_ = dialog.AutoStart();
}

void AppsView::OnAppUpdateRequested()
{
  auto items = ui->app_list->selectedItems();
  if (items.empty()) {
    return;
  }
  int index = ui->app_list->row(items[0]);
  auto* app = app_manager_.AppAtIndex(index);

  if (app->repo_.name_.isEmpty()){
      return;
  }

  RepoRelease release = ApiHandler::GetLatestRelease(app->repo_);

  if (release.version_ == app->version_) {
    QMessageBox::information(nullptr, "", "The app is up to date");
    return;
  }
  // should probably warn about potentially losing settings
  auto result = QMessageBox::question(
      nullptr, "", "A new version of " + app->name_ + " is available, would you like to update?\n The current version will be removed and you may lose your settings");
  if (result == QMessageBox::No) {
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

  if (app_path.isEmpty()){
      QMessageBox::critical(nullptr, "", "Unable to download or extract update.\n Please try again later.");
      return;
  }
  app->executable_path_ = app_path;
  app->version_ = release.version_;

  result = QMessageBox::question(nullptr, "", "Update Complete. Would you like to open " + app->name_ + " ?");
  if (result == QMessageBox::Yes){
      app->Run();
  }
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

void AppsView::OnAppShowRequested()
{
  auto items = ui->app_list->selectedItems();
  if (items.empty()) {
    return;
  }
  int index = ui->app_list->row(items[0]);
  auto* app = app_manager_.AppAtIndex(index);
  app->Show();
}

void AppsView::OnAppMinimizeRequested()
{
  auto items = ui->app_list->selectedItems();
  if (items.empty()) {
    return;
  }
  int index = ui->app_list->row(items[0]);
  auto* app = app_manager_.AppAtIndex(index);
  app->Minimize();
}

void AppsView::OnAppDoubleClicked(QListWidgetItem* item)
{
  int index = ui->app_list->row(item);
  auto* app = app_manager_.AppAtIndex(index);
  if (app != nullptr) {
    if (app->IsRunning()) {
      app->Stop();
    } else {
      app->Run();
    }
  }
}
