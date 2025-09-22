#include <QCheckBox>
#include <QFileDialog>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#include "SettingsDialog.h"
#include "qlabel.h"

using namespace rabbit_App;

SettingsDialog::SettingsDialog(ProjectManager *project_manager, QWidget *parent)
    : QDialog(parent), project_manager_(project_manager) {

  initUi();
  initConnections();
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::initUi() {
  QLabel *project_name_label = new QLabel(tr("Project Name:"), this);
  QLabel *project_name_label_ =
      new QLabel(project_manager_->getProjectName(), this);
  // project_name_edit_ = new QLineEdit(this);
  // project_name_edit_->setText(project_manager_->getProjectName());

  QLabel *project_path_label = new QLabel(tr("Project Path:"), this);
  QLabel *project_path_label_ =
      new QLabel(project_manager_->getProjectPath(), this);
  // project_path_edit_ = new QLineEdit(this);
  // project_path_edit_->setText(project_manager_->getProjectPath());
  // project_path_browse_btn_ = new QPushButton(tr("Browse"), this);

  QLabel *constraint_path_label = new QLabel(tr("Constraint Path"), this);
  constraint_path_edit_ = new QLineEdit(this);
  constraint_path_edit_->setText(project_manager_->getConstraintPath());
  constraint_path_browse_btn_ = new QPushButton(tr("Browse"), this);

  QLabel *bitstream_path_label = new QLabel(tr("Bitstream Path"), this);
  bitstream_path_edit_ = new QLineEdit(this);
  bitstream_path_edit_->setText(project_manager_->getBitstreamPath());
  bitstream_path_browse_btn_ = new QPushButton(tr("Browse"), this);

  QLabel *waveform_label = new QLabel(tr("Generate Waveform"), this);
  generate_waveform_checkbox_ = new QCheckBox(this);
  generate_waveform_checkbox_->setChecked(
      project_manager_->isWaveformEnabled());

  ok_btn_ = new QPushButton(tr("OK"), this);
  cancel_btn_ = new QPushButton(tr("Cancel"), this);

  hint_label_ = new QLabel(this);

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(project_name_label, 0, 0);
  layout->addWidget(project_name_label_, 0, 1);
  layout->addWidget(project_path_label, 1, 0);
  layout->addWidget(project_path_label_, 1, 1);
  layout->addWidget(constraint_path_label, 2, 0);
  layout->addWidget(constraint_path_edit_, 2, 1);
  layout->addWidget(constraint_path_browse_btn_, 2, 2);
  layout->addWidget(bitstream_path_label, 3, 0);
  layout->addWidget(bitstream_path_edit_, 3, 1);
  layout->addWidget(bitstream_path_browse_btn_, 3, 2);
  layout->addWidget(waveform_label, 4, 0);
  layout->addWidget(generate_waveform_checkbox_, 4, 1);
  layout->addWidget(hint_label_, 5, 0);
  layout->setRowStretch(5, 1);
  layout->addWidget(ok_btn_, 6, 1);
  layout->addWidget(cancel_btn_, 6, 2);
  setLayout(layout);
}

void SettingsDialog::initConnections() {
  // connect(project_manager_, &ProjectManager::updateProjectName, this,
  // &SettingsDialog::updateProjectInfo);

  connect(constraint_path_browse_btn_, &QPushButton::clicked, this, [=]() {
    QString constraint_path = QFileDialog::getOpenFileName(
        this, tr("Open File"), project_manager_->getConstraintPath(),
        tr("Constraint Files (*.xml)"));
    constraint_path_edit_->setText(constraint_path);
  });
  connect(bitstream_path_browse_btn_, &QPushButton::clicked, this, [=]() {
    QString bitstream_path = QFileDialog::getOpenFileName(
        this, tr("Open File"), project_manager_->getBitstreamPath(),
        tr("Bitstream Files (*.bit)"));
    bitstream_path_edit_->setText(bitstream_path);
  });
  connect(ok_btn_, &QPushButton::clicked, this, [=]() {
    // project_manager_->setProjectName(project_name_label_->text());
    // project_manager_->setProjectPath(project_path_label_->text());
    project_manager_->setConstraintPath(constraint_path_edit_->text());
    project_manager_->setBitstreamPath(bitstream_path_edit_->text());
    project_manager_->setWaveformEnabled(
        generate_waveform_checkbox_->isChecked());
    // project_manager_->refleshProject(project_manager_->getProjectPath());
    project_manager_->setUnsaved(true);
    accept();
  });
  connect(cancel_btn_, &QPushButton::clicked, this, [=]() { reject(); });
}

void SettingsDialog::showEvent(QShowEvent *event) { QDialog::showEvent(event); }

// void SettingsDialog::updateProjectInfo(const QString &project_name) {
//   project_name_label_->setText(project_manager_->getProjectName());
//   project_path_label_->setText(project_manager_->getProjectPath());
//   constraint_path_edit_->setText(project_manager_->getConstraintPath());
//   bitstream_path_edit_->setText(project_manager_->getBitstreamPath());
// }
