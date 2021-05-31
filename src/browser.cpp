#include "browser.hpp"

#include <QVBoxLayout>

Browser::Browser(QWidget* parent) : QWidget(parent), web_view_(this)
{
  setLayout(new QVBoxLayout());
  layout()->setMargin(0);
  layout()->addWidget(&web_view_);

  web_view_.load(QUrl("https://poe.ninja"));
  web_view_.show();
}
