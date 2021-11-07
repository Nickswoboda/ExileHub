#pragma once

#include <QWidget>

#include "app.hpp"

namespace Ui
{
class AppCard;
}

class AppCard : public QWidget
{
  Q_OBJECT

public:
  explicit AppCard(App& app, QWidget* parent = nullptr);
  ~AppCard();

signals:
  void RemoveAppRequested();
  void AppUpdateRequested();
private slots:
  void on_start_button_clicked(bool checked);

  void on_stop_button_clicked(bool checked);

  void on_min_button_clicked(bool checked);

  void on_max_button_clicked(bool checked);

  void on_action_update_triggered();

  void on_action_remove_triggered();

  void on_action_edit_triggered();

private:
  Ui::AppCard* ui;

  App& app_;
};
