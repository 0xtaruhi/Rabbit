#pragma once
#ifndef MAIN_TAB_TOOL_BAR_H
#define MAIN_TAB_TOOL_BAR_H

#include <QAction>
#include <QMenu>
#include <QSpinBox>

#include "Components/AbstractComponent.h"

#include "TabToolbar/Builder.h"
#include "TabToolbar/Group.h"
#include "TabToolbar/TabToolbar.h"
#include "qtmetamacros.h"

namespace rabbit_App {

class MainTabToolBar : public tt::TabToolbar {
  Q_OBJECT

public:
  MainTabToolBar(QWidget *parent = nullptr, unsigned _groupMaxHeight = 75,
                 unsigned _groupRowCount = 3);
  virtual ~MainTabToolBar();

  void setRunningState(bool is_running);

signals:
  void newProjectClicked();
  void openProjectClicked();
  void saveProjectClicked();
  void saveProjectAsClicked();
  void settingsClicked();
  void runClicked();
  void stopClicked();
  void frequencyChanged(int frequency);
  void waveFormClicked();
  void downloadBitstreamClicked();
  void aboutClicked();
  void helpClicked();
  void appendComponent(component::AbstractComponent *component, int x_pos,
                       int y_pos);

private:
  static const int max_input_action_count = 4;
  static const int max_output_action_count = 4;

  QAction *run_action_;
  QAction *stop_action_;
  QAction *waveform_action_;
  QMenu *input_components_menu_;
  QMenu *output_components_menu_;
  QList<tt::Group *> all_input_groups_;
  QList<tt::Group *> all_output_groups_;

  QSpinBox *frequency_spin_box_;

  void initActions();
  void initMenu();
  void setMenu();

  tt::Builder *builder_;

}; // class MainTabToolBar

} // namespace rabbit_App

#endif // MAIN_TAB_TOOL_BAR_H