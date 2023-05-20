#pragma once
#ifndef NEW_PROJECT_DIALOG_H
#define NEW_PROJECT_DIALOG_H

#include <QPushButton>
#include <QWidget>

namespace rabbit_App {

class NewProjectDialog : public QWidget {
  Q_OBJECT

public:
  NewProjectDialog(QWidget *parent = nullptr);
  ~NewProjectDialog();

private:
  QPushButton *create_project_btn_;
  QPushButton *cancel_btn_;

  void initMembers();
  void initLayout();
};
} // namespace rabbit_App

#endif // NEW_PROJECT_DIALOG_H
