#include "main_window.hpp"

#include "./ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::Application)
{
  ui->setupUi(this);

  setWindowIcon(QIcon(":/assets/icon.png"));
  setWindowTitle("ExileHub");
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::changeEvent(QEvent* event)
{
  if (event->type() == QEvent::WindowStateChange) {
    if (isMinimized()) {
      emit Minimized();
      hide();
    }
  }
  QMainWindow::changeEvent(event);
}
