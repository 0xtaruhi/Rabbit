#include <QApplication>
#include <QDrag>
#include <QEvent>
#include <QGraphicsEffect>
#include <QLayout>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>

#include "Components/AbstractComponent.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/ComponentTitleBar.h"

#include "Ports/Port.h"
#include "Utils.h"
#include "qnamespace.h"
#include "qpalette.h"
#include "qwidget.h"

using namespace rabbit_App::component;

AbstractRawComponent::AbstractRawComponent(QWidget *parent) : QWidget(parent) {}

AbstractRawComponent::~AbstractRawComponent() {}

void AbstractRawComponent::appendPort(QVector<ports::Port> &map,
                                      const QString &name,
                                      const ports::PortType type) {
  auto port = ports::Port();
  port.name = name;
  port.type = type;
  port.pin_index = map.size();
  map.push_back(port);
}

rabbit_App::ports::Port &
AbstractRawComponent::findPort(const QString &port_name,
                               const ports::PortType type) {
  auto &vec = type == ports::PortType::Input ? input_ports_ : output_ports_;
  for (auto &port : vec) {
    if (port.name == port_name) {
      return port;
    }
  }
  throw std::runtime_error("Could not find port");
}

AbstractComponent::AbstractComponent(QWidget *parent) : QWidget(parent) {
  setMouseTracking(true);
  setContextMenuPolicy(Qt::DefaultContextMenu);
  uuid_ = QUuid::createUuid();
  initMembers();
  initLayout();
  initConnections();

  auto shadow_effect = new QGraphicsDropShadowEffect(this);
  shadow_effect->setBlurRadius(10);
  shadow_effect->setOffset(0, 0);
  auto color = QPalette().color(QPalette::Text);
  shadow_effect->setColor(color);
  setGraphicsEffect(shadow_effect);
}

AbstractComponent::~AbstractComponent() {}

void AbstractComponent::initMembers() {
  component_name_ = kDefauleName;
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
  connect(title_bar_, &ComponentTitleBar::settingsBtnClicked, this,
          &AbstractComponent::onSettingsBtnClicked);
}

uint64_t AbstractComponent::getWriteData() const {
  return raw_component_->getWriteData();
}

void AbstractComponent::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    drag_start_position_ = event->pos();
  }
}

void AbstractComponent::keyPressEvent(QKeyEvent *event) {

}

void AbstractComponent::keyReleaseEvent(QKeyEvent *event) {
  
}

void AbstractComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QPalette palette;
  QColor color = palette.color(QPalette::Window);
  color.setAlpha(200);
  painter.setBrush(color);

  painter.setPen(Qt::NoPen);
  painter.drawRoundedRect(0, 0, this->width(), this->height(), 5, 5);
}

void AbstractComponent::contextMenuEvent(QContextMenuEvent *event) {
  QMenu menu;
  menu.addAction(tr("Delete"), this, &AbstractComponent::askForRemove);
  menu.exec(event->globalPos());
}

void AbstractComponent::mouseMoveEvent(QMouseEvent *event) {
  if (!(event->buttons() & Qt::LeftButton)) {
    return;
  }
  if ((event->pos() - drag_start_position_).manhattanLength() >=
      QApplication::startDragDistance()) {
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    auto grab = this->grab();
    setTransparentPixmap(grab, 128);

    drag->setPixmap(grab);
    drag->setHotSpot(event->pos());

    auto data = QByteArray();
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream << uuid_.toString() << drag_start_position_;
    mimeData->setData("application/x-abstractcomponent", data);

    drag->setMimeData(mimeData);
    drag->exec();
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

void AbstractComponent::onSettingsBtnClicked() {
  ComponentSettingsDialog *dialog = new ComponentSettingsDialog(this, this);
  dialog->exec();
  delete dialog;
  // this is not a good way to do this
  // but it works for now, and it's easy to implement
  // to do this properly, we need to implement a settings dialog member in each
  // component class
}
