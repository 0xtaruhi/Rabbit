#include "MainWindow.h"
#include "TabToolbar/Group.h"
#include "TabToolbar/Page.h"
#include "TabToolbar/TabToolbar.h"
#include "qaction.h"
#include "qpushbutton.h"
#include "qtoolbutton.h"
#include <QAction>

using namespace rabbit_App;

constexpr auto kMinWindowWidth = 800;
constexpr auto kMinWindowHeight = 600;
constexpr auto kDefaultStyle = "Kool";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  initMembers();
  initLayout();
}

MainWindow::~MainWindow() {}

void MainWindow::initMembers() {
  builder_ = new tt::Builder(this);
  builder_->SetCustomWidgetCreator("pushButton",
                                   []() { return new QPushButton(); });
  tab_tool_bar_ = new tt::TabToolbar(this, 75, 3);
  status_bar_ = new QStatusBar(this);
  status_bar_->addPermanentWidget(
      new QLabel(tr("USB Disconnected"), status_bar_));
  initActions();

  new_project_dialog_ = new NewProjectDialog(this);
}

void MainWindow::initActions() {
  tt::Page *file_page = tab_tool_bar_->AddPage("File");
  tt::Group *file_group = file_page->AddGroup("Project");

  auto new_project_action = new QAction(tr("New"), this);
  new_project_action->setIcon(QIcon(":/icons/icons/icons8-add-file-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, new_project_action);
  connect(new_project_action, &QAction::triggered, this,
          &MainWindow::onNewProjectClicked);

  auto open_project_action = new QAction(tr("Open"), this);
  open_project_action->setIcon(QIcon(":/icons/icons/icons8-folder-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, open_project_action);
  connect(open_project_action, &QAction::triggered, this,
          &MainWindow::onOpenProjectClicked);

  auto save_project_action = new QAction(tr("Save"), this);
  save_project_action->setIcon(QIcon(":/icons/icons/icons8-save-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, save_project_action);
  connect(save_project_action, &QAction::triggered, this,
          &MainWindow::onOpenProjectClicked);

  auto save_as_project_action = new QAction(tr("Save As"), this);
  save_as_project_action->setIcon(QIcon(":/icons/icons/icons8-save-as-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, save_as_project_action);
  connect(save_as_project_action, &QAction::triggered, this,
          &MainWindow::onOpenProjectClicked);
}

void MainWindow::initLayout() {
  setMinimumSize(kMinWindowWidth, kMinWindowHeight);
  setWindowTitle("Rabbit");

  tab_tool_bar_->SetStyle(kDefaultStyle);
  addToolBar(Qt::TopToolBarArea, tab_tool_bar_);
  setStatusBar(status_bar_);
}

void MainWindow::initConnections() {}

void MainWindow::onNewProjectClicked() {
  new_project_dialog_->exec();
}

void MainWindow::onOpenProjectClicked() {}
