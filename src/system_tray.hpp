#pragma once

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

private slots:
  void OnActivated(QSystemTrayIcon::ActivationReason reason);
  void RestoreMainWindow();
};
