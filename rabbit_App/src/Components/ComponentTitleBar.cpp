#include "Components/ComponentTitleBar.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qsizepolicy.h"
#include "qwidget.h"
#include <QLayout>

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

  delete_btn_ = new QPushButton(this);
  delete_btn_->setFlat(true);
  delete_btn_->setStyleSheet("background-color: red;");
  delete_btn_->setText("X");
  delete_btn_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  font.setPointSize(7);
  delete_btn_->setFont(font);
  delete_btn_->setFixedSize(15,15);

  connect(delete_btn_, &QPushButton::clicked, this,
          &ComponentTitleBar::onDeleteBtnClicked);
}

void ComponentTitleBar::initLayout() {
  auto layout = new QHBoxLayout(this);
  layout->addWidget(title_label_);
  layout->addStretch();
  layout->addWidget(delete_btn_);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}

void ComponentTitleBar::onDeleteBtnClicked() {
  emit deleteBtnClicked();
}
