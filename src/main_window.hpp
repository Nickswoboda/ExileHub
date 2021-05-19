#pragma once

#include <QMainWindow>

#include "system_tray.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
class Application;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  void changeEvent(QEvent* event) override;

  SystemTray system_tray_;
  Ui::Application* ui;
};
