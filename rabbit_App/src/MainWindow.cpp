#include <QAction>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <QUrl>
#include <exception>

#include "Components/ComponentAction.h"
#include "Components/Components.h"
#include "Components/PanelGuiUpdateController.h"
#include "FPGA/AsyncVLFDReadWrite.h"
#include "FPGA/VLFDDeviceHandler.h"
#include "MainWindow.h"
#include "ProjectManager.h"
#include "SettingsDialog.h"
#include "WaveForm/WaveFormController.h"
#include "qmessagebox.h"

using namespace rabbit_App;
using namespace rabbit_App::component;

constexpr auto kMinWindowWidth = 800;
constexpr auto kMinWindowHeight = 600;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  initMembers();
  initLayout();
  initConnections();
  try {
    // vlfd_device_detector_->checkConnection();
    vlfd_device_detector_->startDetect();
  } catch (const std::exception &e) {
    QMessageBox::warning(this, tr("USB Connection"), e.what());
  }
  // set current thread priority to high
  QThread::currentThread()->setPriority(QThread::HighPriority);
  setWindowIcon(QIcon(":/RabbitIcon/RabbitIcon/Rabbit-2048-2048.ico"));
  // qDebug() << "MainWindow thread: " << QThread::currentThreadId();
}

MainWindow::~MainWindow() {}

void MainWindow::initMembers() {
  is_running_ = false;
  main_tab_tool_bar_ = new MainTabToolBar(this);
  status_bar_ = new QStatusBar(this);
  usb_connection_label_ = new QLabel(status_bar_);
  status_bar_->addPermanentWidget(usb_connection_label_);

  components_panel_ = new ComponentsPanel(this);

  waveform_controller_ = new waveform::WaveFormController(this);

  // fpga_manager_ = new FPGAManager(this);
  vlfd_device_handler_ = new fpga::VLFDDeviceHandler(this);
  vlfd_device_handler_->onFrequencyChanged(1);
#ifdef _WIN32
  vlfd_device_detector_ = new fpga::WinVLFDDeviceDetector(this);
#else
  vlfd_device_detector_ = new fpga::LibusbVLFDDeviceDetector(this);
#endif
  project_manager_ = new ProjectManager(this);
  panel_gui_update_controller_ = new PanelGuiUpdateController(this);
  value_update_controller_ = new ValueUpdateController(components_panel_, this);

  project_manager_->setComponentsPanel(components_panel_);
  waveform_controller_->setWaveformEnabled(
      project_manager_->isWaveformEnabled());
}

void MainWindow::initLayout() {
  setMinimumSize(kMinWindowWidth, kMinWindowHeight);
  setWindowTitle("Rabbit");

  addToolBar(Qt::TopToolBarArea, main_tab_tool_bar_);
  setStatusBar(status_bar_);

  setCentralWidget(components_panel_);
  // components_panel_->appendComponent(new LEDComponent(), 1, 1);
}

void MainWindow::initConnections() {
#ifdef _WIN32
  connect(vlfd_device_detector_, &fpga::WinVLFDDeviceDetector::deviceDetected,
          this, &MainWindow::onDeviceDetected);
  connect(vlfd_device_detector_, &fpga::WinVLFDDeviceDetector::deviceRemoved,
          this, &MainWindow::onDeviceRemoved);
#else
  connect(vlfd_device_detector_,
          &fpga::LibusbVLFDDeviceDetector::deviceDetected, this,
          &MainWindow::onDeviceDetected);
  connect(vlfd_device_detector_, &fpga::LibusbVLFDDeviceDetector::deviceRemoved,
          this, &MainWindow::onDeviceRemoved);
#endif

  connect(vlfd_device_handler_,
          &fpga::VLFDDeviceHandler::downloadBitstreamSuccess, this,
          &MainWindow::onDownloadBitstreamSuccess);
  connect(vlfd_device_handler_,
          &fpga::VLFDDeviceHandler::downloadBitstreamFailure, this,
          &MainWindow::onDownloadBitstreamFailure);
  connect(vlfd_device_handler_, &fpga::VLFDDeviceHandler::askForWriteData, this,
          &MainWindow::onAskForWriteData);

  connect(main_tab_tool_bar_, &MainTabToolBar::frequencyChanged,
          vlfd_device_handler_, &fpga::VLFDDeviceHandler::onFrequencyChanged);
  connect(main_tab_tool_bar_, &MainTabToolBar::frequencyChanged,
          waveform_controller_, &waveform::WaveFormController::setFrequency);
  connect(main_tab_tool_bar_, &MainTabToolBar::downloadBitstreamClicked, this,
          &MainWindow::onDownloadBitstreamClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::runClicked, this,
          &MainWindow::onRunningStartClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::stopClicked, this,
          &MainWindow::onRunningStopClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::appendComponent, this,
          [this](AbstractComponent *component) {
            this->components_panel_->appendComponent(component);
          });
  connect(main_tab_tool_bar_, &MainTabToolBar::newProjectClicked, this,
          &MainWindow::onNewProjectClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::openProjectClicked, this,
          &MainWindow::onOpenProjectClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::saveProjectClicked, this,
          &MainWindow::onSaveProjectClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::saveProjectAsClicked, this,
          &MainWindow::onSaveProjectAsClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::settingsClicked, this,
          &MainWindow::onSettingsClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::waveFormClicked, this,
          &MainWindow::onWaveFormClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::aboutClicked, this,
          &MainWindow::onAboutClicked);
  connect(main_tab_tool_bar_, &MainTabToolBar::helpClicked, this,
          &MainWindow::onHelpClicked);

  connect(vlfd_device_handler_->ayncVLFDReadWriteHandler(),
          &fpga::AsyncVLFDReadWrite::readWriteDone, this,
          &MainWindow::onReadWriteDone);
  connect(vlfd_device_handler_->ayncVLFDReadWriteHandler(),
          &fpga::AsyncVLFDReadWrite::readWriteError, this,
          &MainWindow::onReadWriteError);

  // connect(vlfd_device_handler_->ayncVLFDReadWriteHandler(),
  //         &fpga::AsyncVLFDReadWrite::readWriteDone, panel_update_controller_,
  //         &component::PanelUpdateController::onReadData);
  // connect(panel_update_controller_,
  //         &component::PanelUpdateController::updatePanel, components_panel_,
  //         &component::ComponentsPanel::onUpdatePanel);
  connect(vlfd_device_handler_->ayncVLFDReadWriteHandler(),
          &fpga::AsyncVLFDReadWrite::readWriteDone, value_update_controller_,
          &component::ValueUpdateController::onReceiveData);
  connect(panel_gui_update_controller_,
          &component::PanelGuiUpdateController::updateGui, components_panel_,
          &component::ComponentsPanel::onUpdateGui);

  connect(project_manager_, &ProjectManager::updateProjectName, this,
          &MainWindow::onProjectNameUpdated);
  connect(project_manager_, &ProjectManager::showUnsaveFlag, this,
          &MainWindow::onProjectUnsaved);
  connect(project_manager_, &ProjectManager::portsLoaded, waveform_controller_,
          &waveform::WaveFormController::setPortsMap);
  connect(project_manager_, &ProjectManager::waveformSettingChanged, this,
          &MainWindow::onWaveformSettingChanged);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (is_running_) {
    QMessageBox::warning(this, tr("Running"),
                         tr("Please stop running before closing the project."));
    event->ignore();
    return;
  }
  if (project_manager_->closeProject()) {
    vlfd_device_detector_->stopDetect();
    event->accept();
  } else
    event->ignore();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S) {
    onSaveProjectClicked();
  }
}

void MainWindow::onNewProjectClicked() { project_manager_->newProject(); }

void MainWindow::onOpenProjectClicked() {
  try {
    project_manager_->onOpenProject();
  } catch (const std::exception &e) {
    QMessageBox msg;
    msg.setText(tr("Open project failed."));
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle(tr("Open project"));
    msg.setDetailedText(e.what());
    msg.exec();
  }
}

void MainWindow::onSaveProjectClicked() { project_manager_->onSaveProject(); }

void MainWindow::onSaveProjectAsClicked() {
  project_manager_->onSaveAsProject();
}

void MainWindow::onSettingsClicked() {
  SettingsDialog *settings_dialog_ = new SettingsDialog(project_manager_, this);
  settings_dialog_->exec();
  delete settings_dialog_;
}

void MainWindow::onDeviceDetected() {
  usb_connection_label_->setText(tr("USB Connected"));
  usb_connection_label_->setStyleSheet(tr("color: green;"));
}

void MainWindow::onDeviceRemoved() {
  usb_connection_label_->setText(tr("USB Disconnected"));
  usb_connection_label_->setStyleSheet(tr("color: red;"));
}

void MainWindow::onDownloadBitstreamClicked() {
  vlfd_device_handler_->program(project_manager_->getBitstreamPath());
}

void MainWindow::onRunningStartClicked() {
  try {
    waveform_controller_->startWriting(project_manager_->getProjectPath());
    components_panel_->resetAllComponents();
    panel_gui_update_controller_->onStartUpdate();
    value_update_controller_->onStartUpdate();
    vlfd_device_handler_->onStartRunning();
  } catch (const std::exception &e) {
    QMessageBox::critical(this, tr("Start running"), e.what());
    return;
  }
  is_running_ = true;
  main_tab_tool_bar_->setRunningState(true);
}

void MainWindow::onRunningStopClicked() {
  try {
    vlfd_device_handler_->onStopRunning();
    value_update_controller_->onStopUpdate();
    components_panel_->resetAllComponents();
    panel_gui_update_controller_->onStopUpdate();
    waveform_controller_->stopWriting();
  } catch (const std::exception &e) {
    QMessageBox::critical(this, tr("Stop running"), e.what());
    return;
  }
  is_running_ = false;
  main_tab_tool_bar_->setRunningState(false);
  // qDebug() << "stop";
}

void MainWindow::onWaveFormClicked() {
  if (is_running_) {
    QMessageBox::warning(this, tr("Running"),
                         tr("Please stop running before opening waveform."));
    return;
  } else if (project_manager_->getProjectPath().isEmpty()) {
    QMessageBox::warning(this, tr("Waveform"),
                         tr("Please open a project before opening waveform."));
    return;
  } else if (!project_manager_->isWaveformEnabled()) {
    QMessageBox::information(this, tr("Waveform"),
                             tr("Waveform generation is disabled. Enable it in "
                                "Settings to view waveforms."));
    return;
  }
  try {
    waveform_controller_->gtkWaveExec(project_manager_->getProjectPath());
  } catch (const std::exception &e) {
    QMessageBox::critical(this, tr("Waveform"), e.what());
    return;
  }
}

void MainWindow::onAboutClicked() {
  QDesktopServices::openUrl(
      QUrl("https://github.com/0xtaruhi/Rabbit/blob/main/README.md"));
}

void MainWindow::onHelpClicked() {
  QDesktopServices::openUrl(QUrl(
      "https://github.com/0xtaruhi/Rabbit/blob/main/doc/RabbitHelpDoc.md"));
}

void MainWindow::onDownloadBitstreamSuccess() {
  QMessageBox msg;
  msg.setText(tr("Download bitstream success."));
  msg.setIcon(QMessageBox::Information);
  msg.setWindowTitle(tr("Download Bitstream"));
  msg.exec();
}

void MainWindow::onDownloadBitstreamFailure(const QString &error_message) {
  QMessageBox msg;
  msg.setText(error_message);
  msg.setIcon(QMessageBox::Critical);
  msg.setWindowTitle(tr("Download Bitstream"));
  msg.exec();
}

void MainWindow::onAskForWriteData() {
  auto write_data = components_panel_->getWriteData();
  vlfd_device_handler_->setWriteData(write_data);
}

void MainWindow::onReadWriteDone(const std::vector<uint16_t> &read_data,
                                 const std::vector<uint16_t> &write_data) {
  waveform_controller_->appendData(read_data, waveform::DataType::Read);
  waveform_controller_->appendData(write_data, waveform::DataType::Write);
}

void MainWindow::onReadWriteError(const QString &error_message) {
  onRunningStopClicked();
  QMessageBox msg;
  msg.setText(error_message);
  msg.setIcon(QMessageBox::Critical);
  msg.setWindowTitle(tr("Read/Write"));
  msg.exec();
}

void MainWindow::onProjectNameUpdated(const QString &project_name) {
  setWindowTitle(project_name + " - Rabbit");
}

void MainWindow::onProjectUnsaved(bool is_unsaved) {
  if (is_unsaved) {
    setWindowTitle(project_manager_->getProjectName() + "* - Rabbit");
  } else {
    setWindowTitle(project_manager_->getProjectName() + " - Rabbit");
  }
}

void MainWindow::onWaveformSettingChanged(bool enabled) {
  waveform_controller_->setWaveformEnabled(enabled);
  if (status_bar_) {
    auto message = enabled ? tr("Waveform generation enabled.")
                           : tr("Waveform generation disabled.");
    status_bar_->showMessage(message, 3000);
  }
}
