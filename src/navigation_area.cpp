#include "navigation_area.hpp"

#include "ui_navigation_area.h"

NavigationArea::NavigationArea(QWidget* parent)
    : QTabWidget(parent), ui(new Ui::NavigationArea)
{
  ui->setupUi(this);

  auto list_widget = ui->listWidget;
  connect(list_widget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
          SLOT(OnUrlDoubleClicked(QListWidgetItem*)));
}

NavigationArea::~NavigationArea()
{
  delete ui;
}

void NavigationArea::OnUrlDoubleClicked(QListWidgetItem* item)
{
  emit UrlSelected(item->text());
}
