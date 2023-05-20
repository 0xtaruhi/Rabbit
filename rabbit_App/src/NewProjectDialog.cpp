#include "NewProjectDialog.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include <QLayout>

using namespace rabbit_App;

NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent) {
  initMembers();
  initLayout();
}

NewProjectDialog::~NewProjectDialog() {}

void NewProjectDialog::initMembers() {
  create_project_btn_ = new QPushButton(tr("Create"), this);
  cancel_btn_ = new QPushButton(tr("Cancel"), this);

  project_name_edit_ = new QLineEdit(this);
  project_name_label_ = new QLabel(tr("Project Name:"), this);
  project_path_edit_ = new QLineEdit(this);
  project_path_label_ = new QLabel(tr("Project Path:"), this);
  create_project_folder_checkbox_ = new QCheckBox(this);
  create_project_folder_label_ = new QLabel(tr("Create Project Folder:"), this);
  constraints_file_edit_ = new QLineEdit(this);
  constraints_file_label_ = new QLabel(tr("Constraints File:"), this);
}

void NewProjectDialog::initLayout() {
  auto *main_layout = new QVBoxLayout(this);
  auto *btn_layout = new QHBoxLayout();

  auto project_group_box = new QGroupBox(tr("Project"), this);

  auto *project_layout = new QVBoxLayout();
  auto *project_name_layout = new QHBoxLayout();
  project_name_layout->addWidget(project_name_label_);
  project_name_layout->addWidget(project_name_edit_);
  auto *project_path_layout = new QHBoxLayout();
  project_path_layout->addWidget(project_path_label_);
  project_path_layout->addWidget(project_path_edit_);
  auto* create_project_folder_layout = new QHBoxLayout();
  create_project_folder_layout->addWidget(create_project_folder_label_);
  create_project_folder_layout->addWidget(create_project_folder_checkbox_);
  project_layout->addLayout(project_name_layout);
  project_layout->addLayout(project_path_layout);
  project_layout->addLayout(create_project_folder_layout);
  project_group_box->setLayout(project_layout);

  auto *constraints_group_box = new QGroupBox(tr("Constraints"), this);
  auto *constraints_layout = new QVBoxLayout();
  auto *constraints_file_layout = new QHBoxLayout();
  constraints_file_layout->addWidget(constraints_file_label_);
  constraints_file_layout->addWidget(constraints_file_edit_);
  constraints_layout->addLayout(constraints_file_layout);
  constraints_group_box->setLayout(constraints_layout);

  btn_layout->addWidget(create_project_btn_);
  btn_layout->addWidget(cancel_btn_);
  main_layout->addWidget(project_group_box);
  main_layout->addWidget(constraints_group_box);
  main_layout->addLayout(btn_layout);
  main_layout->setContentsMargins(10, 10, 10, 10);
}

void NewProjectDialog::initConnections() {
  connect(create_project_btn_, &QPushButton::clicked, this, &QDialog::accept);
  connect(cancel_btn_, &QPushButton::clicked, this, &QDialog::reject);
}
