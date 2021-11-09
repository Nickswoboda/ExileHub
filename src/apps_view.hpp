#pragma once

#include <QListWidgetItem>
#include <QWidget>

#include "api_handler.hpp"
#include "app_manager.hpp"

namespace Ui
{
class AppsView;
}

class AppsView : public QWidget
{
  Q_OBJECT

public:
  explicit AppsView(AppManager& app_manager, QWidget* parent = nullptr);
  ~AppsView();

  void Init();

private:
  Ui::AppsView* ui;

  AppManager& app_manager_;

  // returns true if app updated
  bool CheckForUpdates(App& app);
  int GetSelectedAssetIndex(const RepoRelease& release) const;
  QString DownloadAndExtractAsset(const Repository& repo,
                                  const ReleaseAsset& asset);
  void InsertAppCard(App* app);

public slots:
  void OnAddAppRequested();
  void OnRemoveAppRequested();
  void OnAppUpdateRequested();
};
