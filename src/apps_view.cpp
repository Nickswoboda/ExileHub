#include "apps_view.hpp"

#include <quazip/JlCompress.h>

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

  connect(&ApiHandler::Instance(),
          SIGNAL(LatestReleaseFound(const Repository&, const QString&,
                                    const QVector<ReleaseAsset>&)),
          this,
          SLOT(OnLatestReleaseFound(const Repository&, const QString&,
                                    const QVector<ReleaseAsset>&)));
  connect(&ApiHandler::Instance(),
          SIGNAL(AssetDownloadComplete(const Repository&, const QString&)),
          this,
          SLOT(OnAssetDownloadComplete(const Repository&, const QString&)));
}

AppsView::~AppsView()
{
  delete ui;
}

void AppsView::OnLatestReleaseFound(const Repository& repo,
                                    const QString& release,
                                    const QVector<ReleaseAsset>& assets)
{
  if (release.isEmpty() || assets.isEmpty()) return;

  // if repo already exists, request came from checking for app update
  // else creating new app

  int asset_index = 0;
  if (assets.size() > 1) {
    QStringList list;
    std::transform(assets.begin(), assets.end(), std::back_inserter(list),
                   [](ReleaseAsset const& asset) { return asset.name_; });
    auto selection = QInputDialog::getItem(nullptr, "Multiple assets found",
                                           "Choose an asset", list);

    asset_index = list.indexOf(selection);
  }

  ApiHandler::Instance().DownloadAsset(repo, assets[asset_index]);
}

void AppsView::OnAssetDownloadComplete(const Repository& repo,
                                       const QString& file_path)
{
  QString dest_folder = "apps/" + repo.author_ + "_" + repo.name_;
  auto executables = ExtractAndMoveFile(file_path, dest_folder);
  if (executables.empty()) {
    QMessageBox::critical(nullptr, "Unable to extract asset file",
                          "Unable to extract asset file");
    return;
  }

  QString executable = executables[0];
  if (executables.size() > 1) {
    auto selection = QInputDialog::getItem(nullptr, "Multiple assets found",
                                           "Choose an asset", executables);

    executable = selection;
  }
  auto& app = app_manager_.AddApp(executable);
  ui->app_list->addItem(app.name_);
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

  if (dialog.RequiresDownload()) {
    Repository repo = ExtractRepoFromUrl(dialog.Path());
    ApiHandler::Instance().GetLatestRelease(repo);
    return;
  }

  auto& app = app_manager_.AddApp(dialog.Path());
  ui->app_list->addItem(app.name_);
  app.detach_on_exit_ = true;
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
