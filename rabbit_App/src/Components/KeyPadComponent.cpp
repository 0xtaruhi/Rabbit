#include <QLayout>
#include <QPainter>
#include <QPalette>

#include "Common.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/KeyPadComponent.h"
#include "qevent.h"

using namespace rabbit_App;
using namespace rabbit_App::component;

Key::Key(QWidget *parent) : QWidget(parent) { setFixedSize(kWidth, kHeight); }

Key::Key(const QString &text, QWidget *parent) : QWidget(parent) {
  text_ = text;
  setFixedSize(kWidth, kHeight);
}

Key::~Key() {}

void Key::setText(const QString &text) {
  text_ = text;
  // update();
}

void Key::setPressed(bool is_pressed) {
  is_pressed_ = is_pressed;
  // update();
}

void Key::setHovered(bool is_hovered) {
  is_hovered_ = is_hovered;
  // update();
}

void Key::setDefault() {
  is_pressed_ = false;
  is_hovered_ = false;
  // update();
}

void Key::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(Qt::NoPen);

  if (is_pressed_) {
    painter.setBrush(kShadowColor);
    painter.drawRoundedRect(0, 0, kWidth, kHeight, kRoundRadius, kRoundRadius);
    painter.setBrush(kPressedColor);
  } else if (is_hovered_) {
    painter.setBrush(kHoverColor);
  } else {
    painter.setBrush(kDefaultColor);
  }

  painter.drawRoundedRect(kBorderWidth, kBorderWidth, kWidth - 2 * kBorderWidth,
                          kHeight - 2 * kBorderWidth, kRoundRadius,
                          kRoundRadius);

  painter.setPen(kCharColor);
  painter.setFont(QFont("Arial", 12));
  painter.drawText(0, 0, kWidth, kHeight, Qt::AlignCenter, text_);
}

void Key::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    is_pressed_ = true;
    // update();
  }
}

void Key::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    is_pressed_ = false;
    // update();
  }
}

void Key::mouseMoveEvent(QMouseEvent *event) {}

void Key::enterEvent(QEnterEvent *event) {
  is_hovered_ = true;
  // update();
}

void Key::leaveEvent(QEvent *event) {
  is_hovered_ = false;
  // update();
}

KeyPadRawComponent::KeyPadRawComponent(QWidget *parent, bool is_4x4)
    : AbstractRawComponent(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  if (is_4x4) {
    setKeyTexts({"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B",
                 "C", "D", "E", "F"});
    setColumnNum(4);
    setRowNum(4);
    initPorts();
    initLayout();
  }
}

KeyPadRawComponent::~KeyPadRawComponent() {}

void KeyPadRawComponent::reset() {
  for (auto &key : keys_map_) {
    key->setDefault();
  }
  update();
}

void KeyPadRawComponent::processReadData(QQueue<uint64_t> &read_queue) {}

uint64_t KeyPadRawComponent::getWriteData() const {
  uint64_t data = 0;
  for (int row = 0; row != row_num_; ++row) {
    auto row_pin_index = input_ports_[row].pin_index;
    for (int column = 0; column != column_num_; ++column) {
      auto key = keys_map_.value({row, column});
      auto column_pin_index = input_ports_[column + row_num_].pin_index;
      auto state = (key->isPressed()) ^ is_low_active_;
      data |= (state << row_pin_index);
      data |= (state << column_pin_index);
    }
  }
  return data;
}

void KeyPadRawComponent::updateAllKeys() {
  for (auto &key : keys_map_) {
    key->update();
  }
}

void KeyPadRawComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(Qt::NoPen);
  painter.setBrush(kBorderColor);
  painter.drawRoundedRect(kShrink, kShrink, width() - 2 * kShrink,
                          height() - 2 * kShrink, kRoundRadius, kRoundRadius);

  painter.setBrush(kBackGroundColor);
  auto distance = kShrink + kBorderWidth;
  painter.drawRoundedRect(distance, distance, width() - 2 * distance,
                          height() - 2 * distance, kRoundRadius - kBorderWidth,
                          kRoundRadius - kBorderWidth);
  // updateAllKeys();
}

void KeyPadRawComponent::initPorts() {
  for (int i = 0; i < row_num_; i++) {
    appendPort(input_ports_, QString("ROW%1").arg(i), ports::PortType::Input);
  }
  for (int i = 0; i < column_num_; i++) {
    appendPort(input_ports_, QString("COL%1").arg(i), ports::PortType::Input);
  }
}

void KeyPadRawComponent::initLayout() {
  auto grid_layout = new QGridLayout(this);
  grid_layout->setContentsMargins(6, 6, 6, 6);
  grid_layout->setSpacing(0);
  for (int i = 0; i != row_num_; ++i) {
    for (int j = 0; j != column_num_; ++j) {
      auto key = new Key(key_texts_[i * column_num_ + j], this);
      grid_layout->addWidget(key, i, j, Qt::AlignCenter);
      keys_map_[{i, j}] = key;
    }
  }
  // this->setLayout(grid_layout);
}

KeyPadComponent::KeyPadComponent(QWidget *parent) : AbstractComponent(parent) {
  raw_component_ = new KeyPadRawComponent(this, true);
  layout()->addWidget(raw_component_);
}

KeyPadComponent::~KeyPadComponent() {}

QSize KeyPadComponent::gridOccupation() const noexcept { return {4, 4}; }

void KeyPadComponent::contextMenuEvent(QContextMenuEvent *event) {
  AbstractComponent::contextMenuEvent(event);
}

SmallKeyPadRawComponent::SmallKeyPadRawComponent(QWidget *parent)
    : KeyPadRawComponent(parent, false) {
  setKeyTexts({"1", "2", "3", "4", "5", "6", "7", "8", "9", "#", "0", "*"});
  setColumnNum(3);
  setRowNum(4);
  initPorts();
  initLayout();
}

SmallKeyPadRawComponent::~SmallKeyPadRawComponent() {}

SmallKeyPadComponent::SmallKeyPadComponent(QWidget *parent)
    : AbstractComponent(parent) {
  raw_component_ = new SmallKeyPadRawComponent(this);
  layout()->addWidget(raw_component_);
}

SmallKeyPadComponent::~SmallKeyPadComponent() {}

QSize SmallKeyPadComponent::gridOccupation() const noexcept { return {3, 4}; }

void SmallKeyPadComponent::contextMenuEvent(QContextMenuEvent *event) {
  AbstractComponent::contextMenuEvent(event);
}

// ACTIVE_ONLY_SETTINGS_BTN_CLICKED(KeyPad)
// ACTIVE_ONLY_SETTINGS_BTN_CLICKED(SmallKeyPad)
// void KeyPadComponent::setSettingsDialog() {
//   settings_dialog_ = new ActiveModeSettingsDialog(this, this);
// }

// void SmallKeyPadComponent::setSettingsDialog() {
//   settings_dialog_ = new ActiveModeSettingsDialog(this, this);
// }

void KeyPadComponent::onSettingsBtnClicked() {
  ComponentSettingsDialog *dialog = new ActiveModeSettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}

void SmallKeyPadComponent::onSettingsBtnClicked() {
  ComponentSettingsDialog *dialog = new ActiveModeSettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}
