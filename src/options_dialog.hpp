#pragma once

#include <QDialog>

namespace Ui
{
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit OptionsDialog(QWidget* parent = nullptr);
  ~OptionsDialog();

private slots:
  void OnAcceptButtonClicked();

private:
  Ui::OptionsDialog* ui;
};
