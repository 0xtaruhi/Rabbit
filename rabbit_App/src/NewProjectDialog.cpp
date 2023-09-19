#include <QBoxLayout>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QLayout>
#include <QLineEdit>
#include <QRegularExpression>

#include "Common.h"
#include "CommonSettings.h"
#include "NewProjectDialog.h"

using namespace rabbit_App;

NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("New Project"));
  setFixedSize(kFixedWidth, kFixedHeight);
  initMembers();
  initLayout();
  initConnections();
}

NewProjectDialog::~NewProjectDialog() {}

void NewProjectDialog::initMembers() {
  is_dir_exist_ = false;
  create_project_btn_ = new QPushButton(tr("Create"), this);
  cancel_btn_ = new QPushButton(tr("Cancel"), this);

  project_name_edit_ = new QLineEdit(this);
  project_name_label_ = new QLabel(tr("Project Name:"), this);
  project_path_edit_ = new QLineEdit(this);
  // project_path_edit_->setText("/home/yang/proj");
  project_path_label_ = new QLabel(tr("Project Path:"), this);
  project_path_browse_btn_ = new QPushButton(tr("Browse"), this);
  create_project_folder_checkbox_ =
      new QCheckBox(tr("Create project folder"), this);
  open_project_checkbox_ =
      new QCheckBox(tr("Open project when setup successfully"), this);
  create_project_folder_checkbox_->setChecked(true);
  open_project_checkbox_->setChecked(true);
  // create_project_folder_label_ = new QLabel(tr("Create Project Folder:"),
  // this);
  constraints_file_edit_ = new QLineEdit(this);
  constraints_file_label_ = new QLabel(tr("Constraints File:"), this);
  constraints_file_browse_btn_ = new QPushButton(tr("Browse"), this);
  bitstream_file_edit_ = new QLineEdit(this);
  bitstream_file_label_ = new QLabel(tr("Bitstream File:"), this);
  bitstream_path_browse_btn_ = new QPushButton(tr("Browse"), this);
  hint_label_ = new QLabel("\n", this);
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
  project_path_layout->addWidget(project_path_browse_btn_);
  auto *checkbox_layout = new QVBoxLayout();
  checkbox_layout->addWidget(create_project_folder_checkbox_);
  checkbox_layout->addWidget(open_project_checkbox_);
  project_layout->addLayout(project_name_layout);
  project_layout->addLayout(project_path_layout);
  project_layout->addLayout(checkbox_layout);
  project_group_box->setLayout(project_layout);

  auto *constraints_group_box =
      new QGroupBox(tr("Constraints & Bitstream"), this);
  auto *constraints_layout = new QVBoxLayout();
  auto *constraints_file_layout = new QHBoxLayout();
  auto *bitstream_path_layout = new QHBoxLayout();
  constraints_file_layout->addWidget(constraints_file_label_);
  constraints_file_layout->addWidget(constraints_file_edit_);
  constraints_file_layout->addWidget(constraints_file_browse_btn_);
  bitstream_path_layout->addWidget(bitstream_file_label_);
  bitstream_path_layout->addWidget(bitstream_file_edit_);
  bitstream_path_layout->addWidget(bitstream_path_browse_btn_);
  constraints_layout->addLayout(constraints_file_layout);
  constraints_layout->addLayout(bitstream_path_layout);
  constraints_group_box->setLayout(constraints_layout);

  btn_layout->addWidget(create_project_btn_);
  btn_layout->addWidget(cancel_btn_);
  main_layout->addWidget(project_group_box);
  main_layout->addWidget(constraints_group_box);
  main_layout->addWidget(hint_label_);
  main_layout->addStretch();
  main_layout->addLayout(btn_layout);
  main_layout->setContentsMargins(10, 10, 10, 10);
}

void NewProjectDialog::initConnections() {
  connect(create_project_btn_, &QPushButton::clicked, this,
          &NewProjectDialog::accept);
  connect(cancel_btn_, &QPushButton::clicked, this, &QDialog::reject);
  connect(project_path_browse_btn_, &QPushButton::clicked, this,
          &NewProjectDialog::onProjectPathBrowseBtnClicked);
  connect(constraints_file_browse_btn_, &QPushButton::clicked, this,
          &NewProjectDialog::onConstraintsFileBrowseBtnClicked);
  connect(bitstream_path_browse_btn_, &QPushButton::clicked, this,
          &NewProjectDialog::onBitstreamFileBrowseBtnClicked);
  connect(project_name_edit_, &QLineEdit::textChanged, this,
          &NewProjectDialog::onLineEditChanged);
  connect(project_path_edit_, &QLineEdit::textChanged, this,
          &NewProjectDialog::onLineEditChanged);
  connect(constraints_file_edit_, &QLineEdit::textChanged, this,
          &NewProjectDialog::onLineEditChanged);
  connect(bitstream_file_edit_, &QLineEdit::textChanged, this,
          &NewProjectDialog::onLineEditChanged);
  connect(create_project_folder_checkbox_, &QCheckBox::stateChanged, this,
          [this](int useless) { onLineEditChanged(""); });
}

void NewProjectDialog::onProjectPathBrowseBtnClicked() {
  auto &settings = CommonSettings::getInstance();
  auto default_rbtprj_dir =
      settings.getSettingsValue("default_rbtprj_dir").toString();
  QString path = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), default_rbtprj_dir,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  project_path_edit_->setText(path);
  settings.updateSettingsValue("default_rbtprj_dir", path);
}

void NewProjectDialog::onConstraintsFileBrowseBtnClicked() {
  QString path = QFileDialog::getOpenFileName(
      this, tr("Open File"),
      is_dir_exist_ ? project_path_edit_->text() : QDir::homePath(),
      tr("Xml Files (*.xml)"));
  constraints_file_edit_->setText(path);
}

void NewProjectDialog::onBitstreamFileBrowseBtnClicked() {
  QString path = QFileDialog::getOpenFileName(
      this, tr("Open File"),
      is_dir_exist_ ? project_path_edit_->text() : QDir::homePath(),
      tr("Bit Files (*.bit)"));
  bitstream_file_edit_->setText(path);
}

void NewProjectDialog::onLineEditChanged(const QString &text) {
  auto name = project_name_edit_->text();
  auto path = project_path_edit_->text();
  if (project_name_edit_->text().isEmpty()) {
    hint_label_->setText(tr("Project name cannot be empty!\n"));
  } else if (name.contains(QRegularExpression("[\\/:*?\"<>|]"))) {
    hint_label_->setText(
        tr("Project name cannot contain special characters!\n"));
  } else if (project_path_edit_->text().isEmpty()) {
    hint_label_->setText(tr("Project path cannot be empty!\n"));
    is_dir_exist_ = false;
  } else if (!QDir(path).exists()) {
    hint_label_->setText(tr("Project path does not exist!\n"));
    is_dir_exist_ = false;
  } else {
    QString create_folder =
        path + "/" + name + "/" + name + kProjectFileExtension;
    QString dont_create_folder = path + "/" + name + kProjectFileExtension;
    hint_label_->setText(tr("Project file will be written into : \n") +
                         (create_project_folder_checkbox_->isChecked()
                              ? create_folder
                              : dont_create_folder));
    hint_label_->setStyleSheet("color: green");
    create_project_btn_->setEnabled(true);
    is_dir_exist_ = true;
    return;
  }
  hint_label_->setStyleSheet("color: red");
  create_project_btn_->setEnabled(false);
}

void NewProjectDialog::accept() { QDialog::accept(); }
