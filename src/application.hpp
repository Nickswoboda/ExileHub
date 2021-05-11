#pragma once

#include <QApplication>
#include <QtNetwork/QNetworkAccessManager>

#include "main_window.hpp"

class Application : public QApplication
{
  Q_OBJECT

public:
  Application(int& argc, char** argv);

private:
  MainWindow main_window_;
  QString version_ = "v1.0.0";
  QNetworkAccessManager network_manager_;

private slots:
  void OnNetworkRequestFinished(QNetworkReply* reply);
};
