#pragma once
#ifndef PROJECT_FILE_HANDLER_H
#define PROJECT_FILE_HANDLER_H

#include <QFile>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Components/Components.h"
#include "Components/ComponentsPanel.h"
#include "Ports/PortsFileReader.h"


namespace rabbit_App {

class ProjectFileHandler {

public:
  ProjectFileHandler();
  virtual ~ProjectFileHandler();

  void readProjectFromFile(const QString &project_path);
  void writeProjectToFile(const QString &project_path);

  const QString &getProjectName() const noexcept { return project_name_; }
  const QString &getProjectPath() const noexcept { return project_path_; }
  const QString &getConstraintPath() const noexcept { return constraint_path_; }
  const QString &getBitstreamPath() const noexcept { return bitstream_path_; }

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
  void
  setComponentsPanel(rabbit_App::component::ComponentsPanel *components_panel) {
    components_panel_ = components_panel;
  }

private:
  rabbit_App::component::ComponentsPanel *components_panel_;

  void readComponentsSettings(
      QXmlStreamReader &xml_reader,
      rabbit_App::component::AbstractComponent *current_component);
  void writeComponentsSettings(
      QXmlStreamWriter &xml_writer,
      rabbit_App::component::AbstractComponent *current_component);

  QString project_name_;
  QString project_path_;
  QString constraint_path_;
  QString bitstream_path_;

}; // class ProjectFileHandler

} // namespace rabbit_App

#endif //  PROJECT_FILE_HANDLER_H