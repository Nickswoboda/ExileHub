#include "main_window.hpp"

#include <QCloseEvent>

#include "./ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), system_tray_(this), ui(new Ui::Application)
{
  ui->setupUi(this);

  setWindowIcon(QIcon(":/assets/icon.png"));
  setWindowTitle("ExileHub");

  connect(&system_tray_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(OnSystemTrayActivated(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::changeEvent(QEvent* event)
{
  if (event->type() == QEvent::WindowStateChange) {
    if (isMinimized()) {
      system_tray_.show();
      hide();
    }
  }
  QMainWindow::changeEvent(event);
}

void MainWindow::OnSystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::ActivationReason::DoubleClick) {
    RestoreFromSystemTray();
  }
}

void MainWindow::RestoreFromSystemTray()
{
  showNormal();
  activateWindow();
  system_tray_.hide();
}
