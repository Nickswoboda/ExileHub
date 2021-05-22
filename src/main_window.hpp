#pragma once

#include <QMainWindow>

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
  void Minimized();

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  void changeEvent(QEvent* event) override;

  Ui::Application* ui;
};
