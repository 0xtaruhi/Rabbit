#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QSpinBox>
#include <QStatusBar>

#include "Components/ComponentsPanel.h"
#include "Components/PanelGuiUpdateController.h"
#include "Components/ValueUpdateController.h"
#include "FPGA/VLFDDeviceDetector.h"
#include "FPGA/VLFDDeviceHandler.h"
#include "WaveForm/WaveFormController.h"
#include "MainTabToolBar.h"
#include "ProjectManager.h"


namespace rabbit_App {
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();

private:
  bool is_running_;
  MainTabToolBar *main_tab_tool_bar_;
  QStatusBar *status_bar_;
  QLabel *usb_connection_label_;
  component::ComponentsPanel *components_panel_;

  rabbit_App::waveform::WaveFormController *waveform_controller_;

  rabbit_App::fpga::VLFDDeviceHandler *vlfd_device_handler_;
  rabbit_App::fpga::VLFDDeviceDetector *vlfd_device_detector_;
  ProjectManager *project_manager_;
  component::PanelGuiUpdateController *panel_gui_update_controller_;
  component::ValueUpdateController *value_update_controller_;

  void initMembers();
  void initLayout();
  void initConnections();

protected:
  // override closeEvent() to save the project before closing the window
  virtual void closeEvent(QCloseEvent *event) override;

  // Ctrl + S to save the project
  virtual void keyPressEvent(QKeyEvent *event) override;

signals:
  void downloadBitstreamClicked(const QString &bitstream_path);
  void frequencyChanged(int frequency);

private slots:
  void onNewProjectClicked();
  void onOpenProjectClicked();
  void onSaveProjectClicked();
  void onSaveProjectAsClicked();

  void onSettingsClicked();

  void onDeviceDetected();
  void onDeviceRemoved();

  void onDownloadBitstreamClicked();
  void onRunningStartClicked();
  void onRunningStopClicked();
  void onWaveFormClicked();

  void onAboutClicked();
  void onHelpClicked();

  void onDownloadBitstreamSuccess();
  void onDownloadBitstreamFailure(const QString &error_message);

  void onAskForWriteData();
  void onReadWriteDone(const std::vector<uint16_t> &read_data, const std::vector<uint16_t> &write_data);
  void onReadWriteError(const QString &error_message);

  void onProjectNameUpdated(const QString &project_name);
  void onProjectUnsaved(bool is_unsaved);
};
} // namespace rabbit_App

#endif // MAINWINDOW_H
