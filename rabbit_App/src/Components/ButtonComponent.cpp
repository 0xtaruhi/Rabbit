#include <QLayout>
#include <QMouseEvent>
#include <cstdint>

#include "Components/AbstractComponent.h"
#include "Components/ButtonComponent.h"
#include "Components/ComponentSettingsDialog.h"
#include "qboxlayout.h"
#include "qgroupbox.h"

using namespace rabbit_App::component;

ButtonRawComponent::ButtonRawComponent(QWidget *parent)
    : AbstractRawComponent(parent) {
  initPorts();
  button_picture_ = &ButtonRawComponent::buttonReleasePicture();
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  button_picture_label_ = new QLabel(this);
  button_picture_label_->setFixedSize(65, 65);
  button_picture_label_->setScaledContents(true);
  button_picture_label_->setPixmap(*button_picture_);
  button_picture_label_->setSizePolicy(QSizePolicy::Ignored,
                                       QSizePolicy::Ignored);
  layout->addWidget(button_picture_label_);

  setLayout(layout);

  initConnections();
}

ButtonRawComponent::~ButtonRawComponent() {}

void ButtonRawComponent::reset() {
  is_pushed_ = false;
  button_picture_ = &ButtonRawComponent::buttonReleasePicture();
  button_picture_label_->setPixmap(*button_picture_);
  update();
}

void ButtonRawComponent::initConnections() {
  connect(this, &ButtonRawComponent::buttonPush, this,
          &ButtonRawComponent::onButtonPush);
  connect(this, &ButtonRawComponent::buttonRelease, this,
          &ButtonRawComponent::onButtonRelease);
}

void ButtonRawComponent::processReadData(QQueue<uint64_t> &read_queue) {}

uint64_t ButtonRawComponent::getWriteData() const {
  uint64_t data = 0;
  // get the pin index
  for (auto input_port : input_ports_) {
    data |= ((is_pushed_ ^ is_low_active_) << input_port.pin_index);
  }
  return data;
}

void ButtonRawComponent::paintEvent(QPaintEvent *event) {}

void ButtonRawComponent::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    is_pushed_ = true;
    emit buttonPush();
  }
}

void ButtonRawComponent::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    is_pushed_ = false;
    emit buttonRelease();
  }
}

void ButtonRawComponent::mouseMoveEvent(QMouseEvent *event) {}

void ButtonRawComponent::initPorts() {
  appendPort(input_ports_, "BTN", ports::PortType::Input);
}

void ButtonRawComponent::onButtonPush() {
  button_picture_ = &ButtonRawComponent::buttonPushPicture();
  button_picture_label_->setPixmap(*button_picture_);
  // update();
}

void ButtonRawComponent::onButtonRelease() {
  button_picture_ = &ButtonRawComponent::buttonReleasePicture();
  button_picture_label_->setPixmap(*button_picture_);
  // update();
}

COMPONENT_CLASS_DEFINITION(Button, 2, 2)

// ACTIVE_ONLY_SETTINGS_BTN_CLICKED(Button)

void ButtonComponent::onSettingsBtnClicked() {
  auto dialog =
      new ComponentSettingsDialogWithFeatures<SettingsFeature::ActiveMode>(
          this, this);
  dialog->exec();
  delete dialog;
}
