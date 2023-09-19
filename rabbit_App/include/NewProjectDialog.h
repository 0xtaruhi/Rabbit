#pragma once
#include "qcheckbox.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#ifndef NEW_PROJECT_DIALOG_H
#define NEW_PROJECT_DIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace rabbit_App {

class NewProjectDialog : public QDialog {
  Q_OBJECT

  static constexpr int kFixedWidth = 480;
  static constexpr int kFixedHeight = 360;

public:
  NewProjectDialog(QWidget *parent = nullptr);
  virtual ~NewProjectDialog();

  const QString projectName() const { return project_name_edit_->text(); }
  const QString projectPath() const { return project_path_edit_->text(); }
  const QString constraintsFilePath() const {
    return constraints_file_edit_->text();
  }
  const QString bitstreamFilePath() const {
    return bitstream_file_edit_->text();
  }
  const bool createProjectFolder() const {
    return create_project_folder_checkbox_->isChecked();
  }
  const bool openProject() const { return open_project_checkbox_->isChecked(); }

private:
  void onProjectPathBrowseBtnClicked();
  void onConstraintsFileBrowseBtnClicked();
  void onBitstreamFileBrowseBtnClicked();
  void onLineEditChanged(const QString &text);

  void accept() override;

private:
  QLineEdit *project_name_edit_;
  QLabel *project_name_label_;
  QLineEdit *project_path_edit_;
  QLabel *project_path_label_;
  QPushButton *project_path_browse_btn_;
  QCheckBox *create_project_folder_checkbox_;
  QCheckBox *open_project_checkbox_;
  // QLabel *create_project_folder_label_;
  QLineEdit *constraints_file_edit_;
  QLabel *constraints_file_label_;
  QPushButton *constraints_file_browse_btn_;
  QLineEdit *bitstream_file_edit_;
  QLabel *bitstream_file_label_;
  QPushButton *bitstream_path_browse_btn_;
  QLabel *hint_label_;

  QPushButton *create_project_btn_;
  QPushButton *cancel_btn_;

  bool is_dir_exist_;

  void initMembers();
  void initLayout();
  void initConnections();
};
} // namespace rabbit_App

#endif // NEW_PROJECT_DIALOG_H
