#include "browser.hpp"

#include <QSettings>
#include <QVBoxLayout>
#include <QWebEngineCookieStore>
#include <QWebEngineProfile>

Browser::Browser(QWidget* parent) : QWidget(parent), web_view_(this)
{
  setLayout(new QVBoxLayout());
  layout()->setMargin(0);
  layout()->addWidget(&web_view_);

  const auto cookie_jar = QWebEngineProfile::defaultProfile()->cookieStore();
  connect(cookie_jar, SIGNAL(cookieAdded(QNetworkCookie)), this,
          SLOT(OnCookieLoaded(QNetworkCookie)));
}

void Browser::LoadUrl(const QString& url)
{
  web_view_.load(QUrl(url));
  web_view_.show();
}

void Browser::OnCookieLoaded(const QNetworkCookie& cookie)
{
  if (cookie.name() == "POESESSID") {
    QSettings settings;
    settings.setValue("POESESSID", cookie.value());
  }
}
