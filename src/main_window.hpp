#pragma once

#include <QMainWindow>

#include "browser.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
class Application;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

signals:
  // emitted to notify SystemTray to show
  void MinimizeToTrayRequested();
  void UpdateCheckRequested();

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private slots:
  void OnOptionsActionTriggered();

private:
  void changeEvent(QEvent* event) override;

  Ui::Application* ui;
  Browser browser_;
};
