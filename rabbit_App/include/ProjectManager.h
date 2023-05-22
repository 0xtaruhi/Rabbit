#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include <QObject>
#include <QString>

namespace rabbit_App {

class ProjectManager : public QObject {
  Q_OBJECT

public:
  ProjectManager(QObject *parent = nullptr);
  ~ProjectManager();

  void readProjectFromFile(const QString &project_path);
  void createProject(const QString &project_name);
  void saveProjectToFile(const QString &project_path);

  void setBitstreamPath(const QString &bitstream_path) {
    bitstream_path_ = bitstream_path;
  }

  QString getBitstreamPath() const noexcept { return bitstream_path_; }

private:
  QString bitstream_path_;
};

} // namespace rabbit_App

#endif // PROJECT_MANAGER_H
