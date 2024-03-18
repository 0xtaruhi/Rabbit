#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QXmlStreamWriter>

#include "Common.h"
#include "Components/ComponentsFactory.h"
#include "Components/ComponentsPanel.h"
#include "NewProjectDialog.h"
#include "Ports/PinInfo.h"

#include "CommonSettings.h"
#include "ProjectManager.h"
#include "Utils.h"

using namespace rabbit_App;

ProjectManager::ProjectManager(QObject *parent) : QObject(parent) {
  ports_file_reader_ = new ports::PortsFileReader(this);
  project_file_handler_ = new ProjectFileHandler();
  is_unsaved_ = false;
}

ProjectManager::~ProjectManager() {}

void ProjectManager::readProjectFromFile(const QString &project_path) {

  project_file_handler_->readProjectFromFile(project_path);
  if (!constraint_path_.isEmpty()) {
    constraint_path_ =
      getAbsolutePath(project_file_handler_->getConstraintPath(), project_path);
    ports_file_reader_->readFromFile(constraint_path_);
    emit portsLoaded(ports_file_reader_->inputs(),
                     ports_file_reader_->outputs());
  }
  project_path_ = project_path;
  project_name_ = project_file_handler_->getProjectName();
  if (!bitstream_path_.isEmpty()) {
    bitstream_path_ =
      getAbsolutePath(project_file_handler_->getBitstreamPath(), project_path);
  }
  is_unsaved_ = false;
  emit updateProjectName(project_name_);
}

void ProjectManager::writeProjectToFile(const QString &project_path) {
  project_file_handler_->setProjectName(project_name_);
  project_file_handler_->setProjectPath(project_path);
  project_file_handler_->setConstraintPath(
      getRelativePath(constraint_path_, project_path));
  project_file_handler_->setBitstreamPath(
      getRelativePath(bitstream_path_, project_path));
  project_file_handler_->writeProjectToFile(project_path);
}

void ProjectManager::createProject(const QString &project_name,
                                   const QString &project_path,
                                   const QString &constraint_path,
                                   const QString &bitstream_path) {
  project_name_ = project_name;
  project_path_ = project_path;
  constraint_path_ = constraint_path;
  bitstream_path_ = bitstream_path;
  saveProject();
}

void ProjectManager::newProject() {
  NewProjectDialog *new_project_dialog = new NewProjectDialog();
  new_project_dialog->exec();
  if (new_project_dialog->result() == QDialog::Rejected) {
    delete new_project_dialog;
    return;
  }
  auto project_name = new_project_dialog->projectName();
  // delete the space in the back of  project path
  auto project_dir_path = new_project_dialog->projectPath().trimmed();
  auto constraints_file_path = new_project_dialog->constraintsFilePath();
  auto bitstream_file_path = new_project_dialog->bitstreamFilePath();
  components_panel_->removeAllComponents();
  if (new_project_dialog->createProjectFolder()) {
    project_dir_path += "/" + project_name;
    QDir dir(project_dir_path);
    if (!dir.exists()) {
      dir.mkpath(".");
    }
  }
  auto project_path =
      project_dir_path + "/" + project_name + kProjectFileExtension;
  // qDebug() << project_name << ", " << project_dir_path << ", " <<
  // project_path << "\n";
  createProject(project_name, project_path, constraints_file_path,
                bitstream_file_path);
  if (new_project_dialog->openProject())
    openProject(project_path);

  delete new_project_dialog;
}

void ProjectManager::openProject() {
  auto &common_settings = CommonSettings::getInstance();
  auto default_rbtprj_dir =
      common_settings.getSettingsValue("default_rbtprj_dir").toString();

  QString project_path = QFileDialog::getOpenFileName(
      nullptr, tr("Open Project"), default_rbtprj_dir,
      tr("Rabbit Project (*.rbtprj)"));

  if (project_path.isEmpty())
    return;
  if (is_unsaved_ && !closeProject())
    return;
  openProject(project_path);
  common_settings.updateSettingsValue("default_rbtprj_dir",
                                      QFileInfo(project_path).absolutePath());
}

void ProjectManager::openProject(const QString &project_path) {
  // try {
  refleshProject(project_path);
  // } catch (std::runtime_error &e) {
  //   throw e;
  // }
  setUnsaved(false);
}

void ProjectManager::refleshProject(const QString &project_path) {
  components_panel_->removeAllComponents();
  readProjectFromFile(project_path);
}

bool ProjectManager::closeProject() {
  if (is_unsaved_) {
    QMessageBox msg(components_panel_);
    msg.setText(tr("The project has been modified."));
    msg.setInformativeText(tr("Do you want to save your changes?"));
    msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                           QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Save);
    msg.setIcon(QMessageBox::Warning);
    msg.setWindowTitle(tr("Save Project"));
    auto ret = msg.exec();
    switch (ret) {
    case QMessageBox::Save:
      saveProject();
      break;
    case QMessageBox::Discard:
      // do nothing
      break;
    case QMessageBox::Cancel:
      return false;
    }
  }
  // TODO : clear the project
  return true;
}

void ProjectManager::saveProject() {
  if (project_path_.isEmpty()) {
    saveAsProject();
    return;
  }
  try {
    writeProjectToFile(project_path_);
  } catch (std::runtime_error &e) {
    throw e;
  }
  setUnsaved(false);
}

void ProjectManager::saveAsProject() {
  QString default_rbtprj_dir = QDir::homePath();

  QString project_path = QFileDialog::getSaveFileName(
      nullptr, tr("Save Project"), default_rbtprj_dir,
      tr("Rabbit Project (*.rbtprj)"));
  if (project_path.isEmpty()) {
    return;
  }

  if (!project_path.endsWith(kProjectFileExtension)) {
    project_path.append(kProjectFileExtension);
  }

  project_path_ = project_path;

  QFileInfo fileInfo(project_path_);

  project_name_ = fileInfo.baseName();

  saveProject();
  emit updateProjectName(project_name_);
}

void ProjectManager::setComponentsPanel(
    component::ComponentsPanel *components_panel) {
  components_panel_ = components_panel;
  project_file_handler_->setComponentsPanel(components_panel);
  connect(components_panel_, &component::ComponentsPanel::componentsModified,
          this, [this]() { this->setUnsaved(true); });
  connect(components_panel_, &component::ComponentsPanel::getConstraintFilePath,
          this, [this]() {
            components_panel_->setConstraintFilePath(constraint_path_);
          });
  connect(components_panel, &component::ComponentsPanel::componentsModified,
          this, [this]() { this->setUnsaved(true); });
}
