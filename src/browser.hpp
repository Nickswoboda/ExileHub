#pragma once

#include <QWidget>
#include <QtWebEngineWidgets/QWebEngineView>

class Browser : public QWidget
{
public:
  Browser(QWidget* parent = nullptr);

  QWebEngineView web_view_;
};
