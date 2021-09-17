#include "options_dialog.hpp"

#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>

#include "ui_options_dialog.h"

OptionsDialog::OptionsDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::OptionsDialog)
{
  ui->setupUi(this);
  setWindowTitle("Options");

  connect(ui->buttonBox, SIGNAL(accepted()), this,
          SLOT(OnAcceptButtonClicked()));
  connect(ui->check_updates_button, SIGNAL(clicked(bool)), parent,
          SIGNAL(UpdateCheckRequested()));
  connect(ui->copy_poesessid_button, SIGNAL(clicked(bool)), this,
          SLOT(OnCopyPoeSessIdClicked()));

  QSettings settings;
  bool auto_update = settings.value("options/auto_update", false).toBool();
  if (auto_update) {
    ui->auto_update_checkbox->setCheckState(Qt::CheckState::Checked);
  }
  bool minimize_to_tray =
      settings.value("options/minimize_to_tray", false).toBool();
  if (minimize_to_tray) {
    ui->minimize_to_tray_checkbox->setCheckState(Qt::CheckState::Checked);
  }
}

OptionsDialog::~OptionsDialog()
{
  delete ui;
}

void OptionsDialog::OnAcceptButtonClicked()
{
  QSettings settings;

  bool auto_update =
      ui->auto_update_checkbox->checkState() == Qt::CheckState::Checked;
  bool minimize_to_tray =
      ui->minimize_to_tray_checkbox->checkState() == Qt::CheckState::Checked;

  settings.setValue("options/auto_update", auto_update);
  settings.setValue("options/minimize_to_tray", minimize_to_tray);
}

void OptionsDialog::OnCopyPoeSessIdClicked()
{
  QSettings settings;

  if (!settings.contains("POESESSID")) {
    QMessageBox::information(this, "No POESESSID found.",
                             "No POESESSID found. You must sign in to "
                             "Pathofexile.com in order to obtain an ID.");
    return;
  }

  auto value = settings.value("POESESSID").toByteArray();
  QUrl test_url("https://pathofexile.com/my-account");
  auto manager_ = new QNetworkAccessManager(this);
  QNetworkRequest request(test_url);

  manager_->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
  request.setRawHeader("Cookie", "POESESSID=" + value);
  QNetworkReply* reply = manager_->get(request);
  connect(reply, SIGNAL(finished()), this, SLOT(CheckPoeSessIdValidity()));
}

void OptionsDialog::CheckPoeSessIdValidity()
{
  qDebug() << "PoeSessId";
  auto reply = dynamic_cast<QNetworkReply*>(sender());
  reply->deleteLater();
  if (reply->error()) {
    qDebug() << reply->errorString();
    return;
  }

  qDebug() << "No Error";
}

void OptionsDialog::OnPoeSessIdValid(bool valid) {}
