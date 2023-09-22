#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include <QObject>
#include <QString>

#include "Components/ComponentsPanel.h"
#include "Ports/Port.h"
#include "Ports/PortsFileReader.h"
#include "ProjectFileHandler.h"
#include "WaveForm/WaveFormController.h"

namespace rabbit_App {

class MainWindow;

class ProjectManager : public QObject {
  Q_OBJECT

  friend class MainWindow;
  friend class SettingsDialog;

public:
  ProjectManager(QObject *parent = nullptr);
  virtual ~ProjectManager();

  auto getProjectName() const noexcept { return project_name_; }
  auto getProjectPath() const noexcept { return project_path_; }
  auto getConstraintPath() const noexcept { return constraint_path_; }
  auto getBitstreamPath() noexcept { return bitstream_path_; }
  auto isUnsaved() const noexcept { return is_unsaved_; }

  void setProjectName(const QString &project_name) {
    project_name_ = project_name;
  }
  void setProjectPath(const QString &project_path) {
    project_path_ = project_path;
  }
  void setConstraintPath(const QString &constraint_path) {
    constraint_path_ = constraint_path;
  }
  void setBitstreamPath(const QString &bitstream_path) {
    bitstream_path_ = bitstream_path;
  }
  void setUnsaved(bool is_unsaved) {
    is_unsaved_ = is_unsaved;
    emit showUnsaveFlag(is_unsaved_);
  }

  void readProjectFromFile(const QString &project_path);
  void writeProjectToFile(const QString &project_path);
  void createProject(const QString &project_name,
                     const QString &project_dir_path,
                     const QString &constraint_path = "",
                     const QString &bitstream_path = "");
  void newProject();
  void openProject();
  void openProject(const QString &project_path);
  void refleshProject(const QString &project_path);
  bool closeProject();
  void saveProject();
  void saveAsProject();

signals:
  void updateProjectName(const QString &project_name);
  void showUnsaveFlag(bool is_unsaved);
  void portsLoaded(const QVector<ports::Port> &inputs,
                   const QVector<ports::Port> &outputs);

public slots:
  void onOpenProject() { openProject(); }
  void onSaveProject() { saveProject(); }
  void onSaveAsProject() { saveAsProject(); }

private:
  QString project_name_;
  QString project_path_;
  QString constraint_path_;
  QString bitstream_path_;

  bool is_unsaved_;

  component::ComponentsPanel *components_panel_;
  ProjectFileHandler *project_file_handler_;
  ports::PortsFileReader *ports_file_reader_;

  void setComponentsPanel(component::ComponentsPanel *components_panel);
};

} // namespace rabbit_App

#endif // PROJECT_MANAGER_H
