#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NewProjectDialog.h"
#include "TabToolbar/Builder.h"

namespace rabbit_App {
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  tt::Builder *builder_;
  tt::TabToolbar* tab_tool_bar_;

  void initMembers();
  void initLayout();
  void initConnections();
  
};
} // namespace rabbit_App

#endif // MAINWINDOW_H
