#pragma once

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

class SystemTray : public QSystemTrayIcon
{
  Q_OBJECT
public:
  explicit SystemTray(QObject* parent = nullptr);

private:
  QMenu context_menu_;
  QIcon icon_;
};
