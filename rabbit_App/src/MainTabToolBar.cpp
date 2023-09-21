#include "MainTabToolBar.h"

#include <QAction>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QWidgetAction>

#include "Components/ComponentAction.h"
#include "Components/Components.h"

#include "TabToolbar/Builder.h"
#include "TabToolbar/Group.h"
#include "TabToolbar/Page.h"
#include "TabToolbar/SubGroup.h"
#include "TabToolbar/TabToolbar.h"
#include "qpainter.h"
#include "qspinbox.h"
#include "qtmetamacros.h"

using namespace rabbit_App;
using namespace rabbit_App::component;

const auto kDefaultStyle = "Vienna";

MainTabToolBar::MainTabToolBar(QWidget *parent, unsigned _groupMaxHeight,
                               unsigned _groupRowCount)
    : tt::TabToolbar(parent) {
  builder_ = new tt::Builder(parent);
  builder_->SetCustomWidgetCreator("pushButton",
                                   []() { return new QPushButton(); });
  this->SetStyle(kDefaultStyle);
  initMenu();
  initActions();
  setMenu();
}

MainTabToolBar::~MainTabToolBar() {
  all_input_groups_.clear();
  all_output_groups_.clear();
}

void MainTabToolBar::setRunningState(bool is_running) {
  if (is_running) {
    run_action_->setDisabled(true);
    stop_action_->setEnabled(true);
    waveform_action_->setDisabled(true);
  } else {
    run_action_->setEnabled(true);
    stop_action_->setDisabled(true);
    waveform_action_->setEnabled(true);
  }
}

void MainTabToolBar::initActions() {
  tt::Page *project_page = AddPage("Project");
  tt::Group *file_group = project_page->AddGroup(tr("File"));

  auto new_project_action = new QAction(tr("New"), this);
  new_project_action->setIcon(QIcon(":/icons/icons/icons8-add-file-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, new_project_action);
  connect(new_project_action, &QAction::triggered, this,
          [this]() { emit newProjectClicked(); });

  auto open_project_action = new QAction(tr("Open"), this);
  open_project_action->setIcon(QIcon(":/icons/icons/icons8-folder-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, open_project_action);
  connect(open_project_action, &QAction::triggered, this,
          [this]() { emit openProjectClicked(); });

  auto save_project_action = new QAction(tr("Save"), this);
  save_project_action->setIcon(QIcon(":/icons/icons/icons8-save-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, save_project_action);
  connect(save_project_action, &QAction::triggered, this,
          [this]() { emit saveProjectClicked(); });

  auto save_as_project_action = new QAction(tr("Save As"), this);
  save_as_project_action->setIcon(QIcon(":/icons/icons/icons8-save-as-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, save_as_project_action);
  connect(save_as_project_action, &QAction::triggered, this,
          [this]() { emit saveProjectAsClicked(); });

  file_group->AddSeparator();

  auto settings_action = new QAction(tr("Settings"));
  settings_action->setIcon(QIcon(":/icons/icons/icons8-setting-94.png"));
  file_group->AddAction(QToolButton::DelayedPopup, settings_action);
  connect(settings_action, &QAction::triggered, this,
          [this]() { emit settingsClicked(); });

  tt::Group *bitstream_group = project_page->AddGroup(tr("Bitstream"));
  auto download_bitstrem_action = new QAction(tr("Download"), this);
  download_bitstrem_action->setIcon(
      QIcon(":/icons/icons/icons8-download-94.png"));
  bitstream_group->AddAction(QToolButton::DelayedPopup,
                             download_bitstrem_action);
  connect(download_bitstrem_action, &QAction::triggered, this,
          [this]() { emit downloadBitstreamClicked(); });

  tt::Group *running_group = project_page->AddGroup(tr("Running"));
  tt::SubGroup *frequency_group =
      running_group->AddSubGroup(tt::SubGroup::Align::No);
  frequency_group->AddWidget(new QLabel(tr("Frequency")));
  frequency_spin_box_ = new QSpinBox();
  frequency_spin_box_->setRange(1, 50000);
  frequency_spin_box_->setSuffix("Hz");
  connect(frequency_spin_box_, QOverload<int>::of(&QSpinBox::valueChanged),
          this, [this](int frequency) { emit frequencyChanged(frequency); });
  //   vlfd_device_handler_->onFrequencyChanged(1);
  frequency_group->AddWidget(frequency_spin_box_);
  running_group->AddSeparator();
  run_action_ = new QAction(tr("Run"), this);
  run_action_->setIcon(QIcon(":/icons/icons/icons8-start-94.png"));
  running_group->AddAction(QToolButton::DelayedPopup, run_action_);
  connect(run_action_, &QAction::triggered, this, [this]() {
    emit runClicked();
    // run_action_->setDisabled(true);
    // stop_action_->setEnabled(true);
  });

  stop_action_ = new QAction(tr("Stop"), this);
  stop_action_->setIcon(QIcon(":/icons/icons/icons8-cancel-94.png"));
  running_group->AddAction(QToolButton::DelayedPopup, stop_action_);
  connect(stop_action_, &QAction::triggered, this, [this]() {
    emit stopClicked();
    // run_action_->setEnabled(true);
    // stop_action_->setDisabled(true);
  });
  stop_action_->setDisabled(true);

  running_group->AddSeparator();
  waveform_action_ = new QAction(tr("Waveform"), this);
  waveform_action_->setIcon(QIcon(":/icons/icons/icons8-waveform-94.png"));
  running_group->AddAction(QToolButton::DelayedPopup, waveform_action_);
  connect(waveform_action_, &QAction::triggered, this,
          [this]() { emit waveFormClicked(); });

  tt::Group *about_group = project_page->AddGroup(tr("About"));
  auto about_action = new QAction(tr("About"), this);
  about_action->setIcon(QIcon(":/icons/icons/icons8-about-94.png"));
  about_group->AddAction(QToolButton::DelayedPopup, about_action);
  connect(about_action, &QAction::triggered, this,
          [this]() { emit aboutClicked(); });

  auto help_action = new QAction(tr("Help"), this);
  help_action->setIcon(QIcon(":/icons/icons/icons8-help-94.png"));
  about_group->AddAction(QToolButton::DelayedPopup, help_action);
  connect(help_action, &QAction::triggered, this,
          [this]() { emit helpClicked(); });

  tt::Page *components_page = AddPage("Components");
  tt::Group *inputs_group = components_page->AddGroup("Inputs");
  tt::Group *outputs_group = components_page->AddGroup("Outputs");

  QList<ComponentAction *> output_component_actions;
  for (const auto &output_component : outputComponents()) {
    output_component_actions.push_back(
        ComponentActionFactory::create(output_component, this));
  }

  QList<ComponentAction *> input_component_actions;
  for (const auto &input_component : inputComponents()) {
    input_component_actions.push_back(
        ComponentActionFactory::create(input_component, this));
  }
  int added_action_count = 0;
  for (auto output : output_component_actions) {
    if (added_action_count < max_output_action_count) {
      outputs_group->AddAction(QToolButton::InstantPopup, output);
    } else {
      if (added_action_count % max_output_action_count == 0) {
        all_output_groups_.push_back(new tt::Group("", this));
      }
      all_output_groups_.back()->AddAction(QToolButton::InstantPopup, output);
    }
    connect(output, &ComponentAction::triggered, this, [this, output]() {
      emit appendComponent(output->createComponent(), 3, 1);
    });
    added_action_count++;
  }

  added_action_count = 0;
  for (auto input : input_component_actions) {
    if (added_action_count < max_input_action_count) {
      inputs_group->AddAction(QToolButton::InstantPopup, input);
    } else {
      if (added_action_count % max_input_action_count == 0) {
        all_input_groups_.push_back(new tt::Group("", this));
      }
      all_input_groups_.back()->AddAction(QToolButton::InstantPopup, input);
    }
    connect(input, &ComponentAction::triggered, this, [this, input]() {
      emit appendComponent(input->createComponent(), 2, 1);
    });
    added_action_count++;
  }

  auto more_input_components = new QAction(tr("more"), this);
  more_input_components->setIcon(QIcon(":/icons/icons/icons8-more-94.png"));
  inputs_group->AddAction(QToolButton::MenuButtonPopup, more_input_components,
                          input_components_menu_);

  auto more_output_components = new QAction(tr("more"), this);
  more_output_components->setIcon(QIcon(":/icons/icons/icons8-more-94.png"));
  outputs_group->AddAction(QToolButton::MenuButtonPopup, more_output_components,
                           output_components_menu_);
}

void MainTabToolBar::initMenu() {
  input_components_menu_ = new QMenu(this);
  output_components_menu_ = new QMenu(this);
}

void MainTabToolBar::setMenu() {
  for (auto input_group : all_input_groups_) {
    QWidgetAction *input_component_action =
        new QWidgetAction(input_components_menu_);
    input_component_action->setDefaultWidget(input_group);
    input_components_menu_->addAction(input_component_action);
  }
  for (auto output_group : all_output_groups_) {
    QWidgetAction *output_component_action =
        new QWidgetAction(output_components_menu_);
    output_component_action->setDefaultWidget(output_group);
    output_components_menu_->addAction(output_component_action);
  }
}
