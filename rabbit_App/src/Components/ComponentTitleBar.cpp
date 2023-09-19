#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QWidget>

#include "Components/ComponentTitleBar.h"

using namespace rabbit_App::component;

ComponentTitleBar::ComponentTitleBar(QWidget *parent, const QString &title)
    : QWidget(parent), title_(title) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  initMembers();
  initLayout();
}

ComponentTitleBar::~ComponentTitleBar() {}

QSize ComponentTitleBar::sizeHint() const { return QSize(100, 10); }

void ComponentTitleBar::setTitle(const QString &title) {
  title_ = title;
  title_label_->setText(title);
}

void ComponentTitleBar::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
}

void ComponentTitleBar::initMembers() {
  title_label_ = new QLabel(this);
  title_label_->setText(title_);
  auto font = title_label_->font();
  font.setPointSize(10);
  title_label_->setFont(font);

  settings_btn_ = new QPushButton(this);
  settings_btn_->setFlat(true);
  settings_btn_->setIcon(QIcon(":/icons/icons/icons8-settings-94.png"));
  settings_btn_->setIconSize(QSize(12, 12));
  // settings_btn_->setText("+");
  settings_btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  // font.setPointSize(7);
  // settings_btn_->setFont(font);
  settings_btn_->setFixedSize(15, 15);
  connect(settings_btn_, &QPushButton::clicked, this,
          &ComponentTitleBar::onSettingsBtnClicked);

  delete_btn_ = new QPushButton(this);
  delete_btn_->setFlat(true);
  // delete_btn_->setStyleSheet("background-color: red;");
  delete_btn_->setIcon(QIcon(":/icons/icons/icons8-delete-94.png"));
  delete_btn_->setIconSize(QSize(12, 12));
  // delete_btn_->setText("X");
  delete_btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  // font.setPointSize(7);
  // delete_btn_->setFont(font);
  delete_btn_->setFixedSize(15, 15);

  connect(delete_btn_, &QPushButton::clicked, this,
          &ComponentTitleBar::onDeleteBtnClicked);
}

void ComponentTitleBar::initLayout() {
  auto layout = new QHBoxLayout(this);
  layout->addWidget(title_label_);
  layout->addStretch();
  layout->addWidget(settings_btn_);
  layout->addWidget(delete_btn_);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  setLayout(layout);
}

void ComponentTitleBar::onDeleteBtnClicked() { emit deleteBtnClicked(); }

void ComponentTitleBar::onSettingsBtnClicked() { emit settingsBtnClicked(); }