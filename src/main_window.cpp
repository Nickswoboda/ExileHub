#include "main_window.hpp"
#include "./ui_main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Application)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

