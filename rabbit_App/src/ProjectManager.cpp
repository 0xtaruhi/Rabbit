#include "ProjectManager.h"
#include "qobject.h"

using namespace rabbit_App;

ProjectManager::ProjectManager(QObject *parent)
    : QObject(parent), bitstream_path_() {}

ProjectManager::~ProjectManager() {}

void ProjectManager::readProjectFromFile(const QString &project_path) {}

void ProjectManager::createProject(const QString &project_name) {}

void ProjectManager::saveProjectToFile(const QString &project_path) {}
