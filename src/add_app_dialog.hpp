#pragma once

#include <QDialog>

namespace Ui
{
class AddAppDialog;
}

class AddAppDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AddAppDialog(QWidget* parent = nullptr);
  ~AddAppDialog();

  bool RequiresDownload() const;
  QString Path() const;
  QString Name() const;
  bool AutoUpdate() const;
  bool AutoStart() const;
  bool Detach() const;

private:
  Ui::AddAppDialog* ui;
};
