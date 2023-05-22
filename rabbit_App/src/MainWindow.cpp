#include <QAction>
#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <exception>

#include "TabToolbar/Group.h"
#include "TabToolbar/Page.h"
#include "TabToolbar/SubGroup.h"
#include "TabToolbar/TabToolbar.h"

#include "FPGAManager.h"
#include "ProjectManager.h"

#include "Components/ComponentAction.h"
#include "Components/Components.h"
#include "MainWindow.h"

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

  fpga_manager_ = new FPGAManager(this);
  project_manager_ = new ProjectManager(this);
}

void MainWindow::initActions() {
  tt::Page *project_page = tab_tool_bar_->AddPage("Project");
  tt::Group *file_group = project_page->AddGroup(tr("File"));

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

  file_group->AddSeparator();

  auto settings_action = new QAction(tr("Settings"));
  settings_action->setIcon(QIcon(":/icons/icons/icons8-setting-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, settings_action);

  tt::Group *bitstream_group = project_page->AddGroup(tr("Bitstream"));
  auto download_bitstrem_action = new QAction(tr("Download"), this);
  download_bitstrem_action->setIcon(
      QIcon(":/icons/icons/icons8-download-94.png"));
  bitstream_group->AddAction(QToolButton::DelayedPopup,
                             download_bitstrem_action);
  connect(download_bitstrem_action, &QAction::triggered, this,
          &MainWindow::onDownloadBitstreamClicked);

  tt::Group *running_group = project_page->AddGroup(tr("Running"));
  tt::SubGroup *frequency_group =
      running_group->AddSubGroup(tt::SubGroup::Align::No);
  frequency_group->AddWidget(new QLabel(tr("Frequency")));
  frequency_spin_box_ = new QSpinBox();
  frequency_spin_box_->setRange(1, 10000);
  frequency_spin_box_->setSuffix("Hz");
  frequency_group->AddWidget(frequency_spin_box_);
  running_group->AddSeparator();
  run_action_ = new QAction(tr("Run"), this);
  run_action_->setIcon(QIcon(":/icons/icons/icons8-start-94.png"));
  running_group->AddAction(QToolButton::DelayedPopup, run_action_);
  connect(run_action_, &QAction::triggered, this,
          &MainWindow::onRunningStartClicked);

  stop_action_ = new QAction(tr("Stop"), this);
  stop_action_->setIcon(QIcon(":/icons/icons/icons8-cancel-94.png"));
  running_group->AddAction(QToolButton::DelayedPopup, stop_action_);
  connect(stop_action_, &QAction::triggered, this,
          &MainWindow::onRunningStopClicked);
  stop_action_->setDisabled(true);

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

void MainWindow::onDownloadBitstreamClicked() {
  try {
    fpga_manager_->program(project_manager_->getBitstreamPath());
  } catch (const std::exception &e) {
    QMessageBox msg;
    msg.setText(e.what());
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle(tr("Program Failed"));
    msg.exec();
  }
}

void MainWindow::onRunningStartClicked() {
  run_action_->setDisabled(true);
  stop_action_->setEnabled(true);
}

void MainWindow::onRunningStopClicked() {
  run_action_->setEnabled(true);
  stop_action_->setDisabled(true);
}
