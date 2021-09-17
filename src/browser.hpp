#pragma once

#include <QString>
#include <QWidget>
#include <QtWebEngineWidgets/QWebEngineView>

class Browser : public QWidget
{
  Q_OBJECT

public:
  Browser(QWidget* parent = nullptr);

  QWebEngineView web_view_;

public slots:
  void LoadUrl(const QString& url);
  void OnCookieLoaded(const QNetworkCookie& cookie);
};
