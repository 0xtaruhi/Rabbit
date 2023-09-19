#include <QLayout>
#include <QMouseEvent>
#include <cstdint>

#include "Components/AbstractComponent.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/SwitchComponent.h"

using namespace rabbit_App::component;

SwitchRawComponent::SwitchRawComponent(QWidget *parent)
    : AbstractRawComponent(parent) {
  initPorts();
  switch_picture_ = &SwitchRawComponent::switchOffPicture();
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  switch_picture_label_ = new QLabel(this);
  switch_picture_label_->setPixmap(*switch_picture_);
  switch_picture_label_->setScaledContents(true);
  switch_picture_label_->setSizePolicy(QSizePolicy::Ignored,
                                       QSizePolicy::Ignored);
  layout->addWidget(switch_picture_label_);

  setLayout(layout);

  initConnections();
}

SwitchRawComponent::~SwitchRawComponent() {}

void SwitchRawComponent::reset() {
  is_on_ = false;
  switch_picture_ = &SwitchRawComponent::switchOffPicture();
  switch_picture_label_->setPixmap(*switch_picture_);
  update();
}

void SwitchRawComponent::initConnections() {
  connect(this, &SwitchRawComponent::switchToggled, this,
          &SwitchRawComponent::onSwitchToggled);
}

void SwitchRawComponent::processReadData(QQueue<uint64_t> &read_queue) {}

uint64_t SwitchRawComponent::getWriteData() const {
  uint64_t data = 0;
  // get the pin index
  for (auto input_port : input_ports_) {
    // set the data according to the index
    data |= ((is_on_ ^ is_low_active_) << input_port.pin_index);
  }
  return data;
}

void SwitchRawComponent::paintEvent(QPaintEvent *event) {}

void SwitchRawComponent::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    emit switchToggled();
  }
}

void SwitchRawComponent::initPorts() {
  appendPort(input_ports_, "SW", ports::PortType::Input);
}

void SwitchRawComponent::onSwitchToggled() {
  is_on_ = !is_on_;
  switch_picture_ = is_on_ ? &SwitchRawComponent::switchOnPicture()
                           : &SwitchRawComponent::switchOffPicture();
  switch_picture_label_->setPixmap(*switch_picture_);
  // update();
}

COMPONENT_CLASS_DEFINITION(Switch, 2, 2)

// ACTIVE_ONLY_SETTINGS_BTN_CLICKED(Switch)

void SwitchComponent::onSettingsBtnClicked() {
  auto settings_dialog = new ActiveModeSettingsDialog(this, this);
  settings_dialog->exec();
  delete settings_dialog;
}