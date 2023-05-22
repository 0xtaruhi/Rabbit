#pragma once
#include "qlineedit.h"
#ifndef NEW_PROJECT_DIALOG_H
#define NEW_PROJECT_DIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

namespace rabbit_App {

class NewProjectDialog : public QDialog {
  Q_OBJECT

public:
  NewProjectDialog(QWidget *parent = nullptr);
  virtual ~NewProjectDialog();

private:
  QLineEdit *project_name_edit_;
  QLabel *project_name_label_;
  QLineEdit *project_path_edit_;
  QLabel *project_path_label_;
  QCheckBox *create_project_folder_checkbox_;
  QLabel *create_project_folder_label_;
  QLineEdit *constraints_file_edit_;
  QLabel *constraints_file_label_;

  QPushButton *create_project_btn_;
  QPushButton *cancel_btn_;

  void initMembers();
  void initLayout();
  void initConnections();
};
} // namespace rabbit_App

#endif // NEW_PROJECT_DIALOG_H
