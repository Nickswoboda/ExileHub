#include "main_window.hpp"

#include <QSettings>

#include "./ui_main_window.h"
#include "options_dialog.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::Application),
      nav_area_(this),
      browser_(this)
{
  ui->setupUi(this);

  setWindowIcon(QIcon(":/assets/icon.png"));
  setWindowTitle("ExileHub");

  connect(ui->actionOptions, SIGNAL(triggered()), this,
          SLOT(OnOptionsActionTriggered()));

  ui->window_layout->addWidget(&nav_area_);
  ui->window_layout->addWidget(&browser_);

  connect(&nav_area_, SIGNAL(UrlSelected(const QString&)), &browser_,
          SLOT(LoadUrl(const QString&)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::OnOptionsActionTriggered()
{
  auto dialog = new OptionsDialog(this);
  dialog->exec();
}

void MainWindow::changeEvent(QEvent* event)
{
  if (event->type() == QEvent::WindowStateChange) {
    if (isMinimized()) {
      QSettings settings;
      if (settings.value("options/minimize_to_tray").toBool()) {
        emit MinimizeToTrayRequested();
      }
    }
  }
  QMainWindow::changeEvent(event);
}
