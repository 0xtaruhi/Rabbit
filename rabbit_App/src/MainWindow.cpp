#include "MainWindow.h"
#include "Components/ComponentAction.h"
#include "Components/Components.h"
#include "TabToolbar/Group.h"
#include "TabToolbar/Page.h"
#include "TabToolbar/TabToolbar.h"
#include "qaction.h"
#include "qpushbutton.h"
#include "qtoolbutton.h"
#include <QAction>

using namespace rabbit_App;
using namespace rabbit_App::component;

constexpr auto kMinWindowWidth = 800;
constexpr auto kMinWindowHeight = 600;
const auto kDefaultStyle = "Vienna";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  initMembers();
  initLayout();
  initActions();
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

  components_panel_ = new ComponentsPanel(this);
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

  tt::Page *components_page = tab_tool_bar_->AddPage("Components");
  tt::Group *inputs_group = components_page->AddGroup("Inputs");
  tt::Group *outputs_group = components_page->AddGroup("Outputs");

  QList<ComponentAction *> output_component_actions;
  output_component_actions.push_back(
      ComponentActionFactory::create("LED", this));

  QList<ComponentAction *> input_component_actions;

  for (auto output : output_component_actions) {
    outputs_group->AddAction(QToolButton::InstantPopup, output);
    connect(output, &ComponentAction::triggered, components_panel_, [=]() {
      components_panel_->appendComponent(output->createComponent(), 3, 1);
    });
  }

  for (auto input : input_component_actions) {
    inputs_group->AddAction(QToolButton::InstantPopup, input);
    connect(input, &ComponentAction::triggered, components_panel_, [=]() {
      components_panel_->appendComponent(input->createComponent(), 2, 1);
    });
  }
}

void MainWindow::initLayout() {
  setMinimumSize(kMinWindowWidth, kMinWindowHeight);
  setWindowTitle("Rabbit");

  tab_tool_bar_->SetStyle(kDefaultStyle);
  addToolBar(Qt::TopToolBarArea, tab_tool_bar_);
  setStatusBar(status_bar_);

  setCentralWidget(components_panel_);
  components_panel_->appendComponent(new LEDComponent(), 1, 1);
}

void MainWindow::initConnections() {}

void MainWindow::onNewProjectClicked() { new_project_dialog_->exec(); }

void MainWindow::onOpenProjectClicked() {}
