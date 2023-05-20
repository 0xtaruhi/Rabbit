#include "NewProjectDialog.h"
#include <QLayout>

using namespace rabbit_App;

NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent) {
  initMembers();
  initLayout();
}

void NewProjectDialog::initMembers() {
  create_project_btn_ = new QPushButton(tr("Create"), this);
  cancel_btn_ = new QPushButton(tr("Cancel"), this);
}

void NewProjectDialog::initLayout() {
  auto *main_layout = new QVBoxLayout(this);
  auto *btn_layout = new QHBoxLayout(this);

  btn_layout->addWidget(create_project_btn_);
  btn_layout->addWidget(cancel_btn_);

  main_layout->addLayout(btn_layout);
  setLayout(main_layout);
}

NewProjectDialog::~NewProjectDialog() {}
