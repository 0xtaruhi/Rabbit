#pragma once
#include "qaction.h"
#include "qspinbox.h"
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Components/ComponentsPanel.h"
#include "NewProjectDialog.h"
#include "TabToolbar/Builder.h"
#include <QLabel>
#include <QMainWindow>
#include <QStatusBar>
#include <QSpinBox>
#include "FPGAManager.h"
#include "ProjectManager.h"

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
  component::ComponentsPanel *components_panel_;
  QSpinBox* frequency_spin_box_;

  // dialog handlers
  NewProjectDialog *new_project_dialog_;

  FPGAManager* fpga_manager_;
  ProjectManager* project_manager_;

  QAction* run_action_;
  QAction* stop_action_;

  void initMembers();

  /// @brief Initializes the actions for the toolbar.
  /// It is called by initMembers(). Do not call it directly.
  void initActions();

  void initLayout();
  void initConnections();

private slots:
  void onNewProjectClicked();
  void onOpenProjectClicked();
  
  void onDownloadBitstreamClicked();
  void onRunningStartClicked();
  void onRunningStopClicked();
};
} // namespace rabbit_App

#endif // MAINWINDOW_H
