#pragma once
#include "qlineedit.h"
#include "qpushbutton.h"
#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

#include "ProjectManager.h"

namespace rabbit_App {

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  SettingsDialog(ProjectManager *project_manager, QWidget *parent);
  virtual ~SettingsDialog();

public slots:

  // void updateProjectInfo(const QString& project_name);

protected:
  /// @brief This method is called when the dialog shows.
  void showEvent(QShowEvent *event) override;

private:
  void initUi();
  void initConnections();

private:
  ProjectManager *project_manager_;

  QLabel *project_name_label_;
  QLabel *project_path_label_;
  QLineEdit *constraint_path_edit_;
  QLineEdit *bitstream_path_edit_;

  QPushButton *constraint_path_browse_btn_;
  QPushButton *bitstream_path_browse_btn_;
  QPushButton *ok_btn_;
  QPushButton *cancel_btn_;

  QLabel *hint_label_;
};

} // namespace rabbit_App

#endif // SETTINGS_DIALOG_H
