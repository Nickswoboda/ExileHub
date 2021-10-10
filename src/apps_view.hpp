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
  Ui::AppsView* ui;

  AppManager& app_manager_;
private slots:
  void OnLatestReleaseFound(const Repository& repo, const QString& release,
                            const QVector<ReleaseAsset>& assets);
  void OnAssetDownloadComplete(const Repository& repo,
                               const QString& file_path);

public slots:
  void OnAddAppRequested();
  void OnAppDoubleClicked(QListWidgetItem* item);
};
