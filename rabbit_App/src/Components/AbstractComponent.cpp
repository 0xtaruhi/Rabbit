#include "Components/AbstractComponent.h"
#include "Components/ComponentTitleBar.h"
#include "Utils.h"
#include "qdebug.h"
#include "qevent.h"
#include "qnamespace.h"
#include <QApplication>
#include <QDrag>
#include <QEvent>
#include <QLayout>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsEffect>

using namespace rabbit_App::component;

AbstractComponent::AbstractComponent(QWidget *parent) : QWidget(parent) {
  setMouseTracking(true);
  uuid_ = QUuid::createUuid();
  initMembers();
  initLayout();
  initConnections();

  auto shadow_effect = new QGraphicsDropShadowEffect(this);
  shadow_effect->setBlurRadius(10);
  shadow_effect->setOffset(0, 0);
  shadow_effect->setColor(QColor(0, 0, 0, 100));
  setGraphicsEffect(shadow_effect);
}

AbstractComponent::~AbstractComponent() {}

void AbstractComponent::initMembers() {
  title_bar_ = new ComponentTitleBar(this);
}

void AbstractComponent::initLayout() {
  auto layout = new QVBoxLayout(this);
  layout->addWidget(title_bar_);
  layout->setContentsMargins(10, 3, 10, 10);
  setLayout(layout);
}

void AbstractComponent::initConnections() {
  connect(title_bar_, &ComponentTitleBar::deleteBtnClicked, this,
          &AbstractComponent::askForRemove);
}

void AbstractComponent::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    drag_start_position_ = event->pos();
  }
}

void AbstractComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QLinearGradient linearGradient(QPointF(0, 0), QPointF(200, 200));
  linearGradient.setColorAt(0, Qt::white);
  linearGradient.setColorAt(1, Qt::lightGray);
  painter.setBrush(linearGradient);
  painter.setPen(
      QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawRoundedRect(0, 0, this->width(), this->height(), 5, 5);
}

void AbstractComponent::mouseMoveEvent(QMouseEvent *event) {
  if (!(event->buttons() & Qt::LeftButton)) {
    return;
  }
  qDebug() << "mouseMoveEvent left button";
  if ((event->pos() - drag_start_position_).manhattanLength() >=
      QApplication::startDragDistance()) {
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    auto grab = this->grab();
    setTransparentPixmap(grab, 128);

    drag->setPixmap(grab);
    drag->setHotSpot(event->pos());

    qDebug() << "event pos" << event->pos();
    qDebug() << "this pos" << this->pos();

    auto data = QByteArray();
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream << uuid_ << (event->pos() - drag_start_position_);
    mimeData->setData("application/x-abstractcomponent", data);

    drag->setMimeData(mimeData);
  }
}

void AbstractComponent::askForRemove() {
  QMessageBox msg_box;
  msg_box.setWindowTitle(tr("Remove component"));
  msg_box.setIcon(QMessageBox::Warning);
  msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msg_box.setText(tr("Do you really want to remove this component?"));
  msg_box.setDefaultButton(QMessageBox::No);
  auto ret = msg_box.exec();
  if (ret == QMessageBox::Yes) {
    emit componentRemoved(this);
  }
}
