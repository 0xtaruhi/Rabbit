#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "NewProjectDialog.h"
#include "TabToolbar/Builder.h"
#include <QLabel>
#include <QMainWindow>
#include <QStatusBar>

namespace rabbit_App {
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  tt::Builder *builder_;
  tt::TabToolbar *tab_tool_bar_;
  QStatusBar *status_bar_;

  // dialog handlers
  NewProjectDialog *new_project_dialog_;

  void initMembers();

  /// @brief Initializes the actions for the toolbar.
  /// It is called by initMembers(). Do not call it directly.
  void initActions();

  void initLayout();
  void initConnections();

private slots:
  void onNewProjectClicked();
  void onOpenProjectClicked();
};
} // namespace rabbit_App

#endif // MAINWINDOW_H
