#pragma once

#include <QListWidgetItem>
#include <QTabWidget>

namespace Ui
{
class NavigationArea;
}

class NavigationArea : public QTabWidget
{
  Q_OBJECT

signals:
  void UrlSelected(const QString& url);

public:
  explicit NavigationArea(QWidget* parent = nullptr);
  ~NavigationArea();

private slots:
  void OnUrlDoubleClicked(QListWidgetItem* item);

private:
  Ui::NavigationArea* ui;
};
