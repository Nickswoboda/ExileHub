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

private:
  void OnLatestReleaseFound(const Repository& repo, const QString& release,
                            const QVector<ReleaseAsset>& assets);
  void OnAssetDownloaded(const Repository& repo, const QString& file_path);
  Ui::AppsView* ui;

  AppManager& app_manager_;

public slots:
  void OnAddAppRequested();
  void OnAppDoubleClicked(QListWidgetItem* item);
};
