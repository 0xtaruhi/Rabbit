#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include <QObject>
#include <QString>

namespace rabbit_App {

class ProjectManager : public QObject {
  Q_OBJECT

public:
  ProjectManager();
  ~ProjectManager();

  void readProjectFromFile(const QString &project_path);
  void createProject(const QString &project_name);
  void saveProjectToFile(const QString &project_path);

private:
};

} // namespace rabbit_App

#endif // PROJECT_MANAGER_H
