#pragma once

#include <QDialog>

namespace Ui
{
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
  Q_OBJECT

signals:
  void PoeSessIdValidityChecked(bool valid);

public:
  explicit OptionsDialog(QWidget* parent = nullptr);
  ~OptionsDialog();

private slots:
  void OnAcceptButtonClicked();
  void OnCopyPoeSessIdClicked();
  void CheckPoeSessIdValidity();
  void OnPoeSessIdValid(bool valid);

private:
  Ui::OptionsDialog* ui;
};
