#include <QLayout>
#include <QMouseEvent>
#include <cstdint>
#include <iostream>

#include "Components/AbstractComponent.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/PS2KeyboardComponent.h"

using namespace rabbit_App::component;

static const QHash<int, int> keyMap{{Qt::Key_Escape, 0x76},
                                    {Qt::Key_F1, 0x05},
                                    {Qt::Key_F2, 0x06},
                                    {Qt::Key_F3, 0x04},
                                    {Qt::Key_F4, 0x0c},
                                    {Qt::Key_F5, 0x03},
                                    {Qt::Key_F6, 0x0b},
                                    {Qt::Key_F7, 0x83},
                                    {Qt::Key_F8, 0x0a},
                                    {Qt::Key_F9, 0x01},
                                    {Qt::Key_F10, 0x09},
                                    {Qt::Key_F11, 0x78},
                                    {Qt::Key_F12, 0x07},

                                    {Qt::Key_1, 0x16},
                                    {Qt::Key_2, 0x1e},
                                    {Qt::Key_3, 0x26},
                                    {Qt::Key_4, 0x25},
                                    {Qt::Key_5, 0x2e},
                                    {Qt::Key_6, 0x36},
                                    {Qt::Key_7, 0x3d},
                                    {Qt::Key_8, 0x3e},
                                    {Qt::Key_9, 0x46},
                                    {Qt::Key_0, 0x45},

                                    {Qt::Key_Exclam, 0x16},
                                    {Qt::Key_At, 0x1e},
                                    {Qt::Key_NumberSign, 0x26},
                                    {Qt::Key_Dollar, 0x25},
                                    {Qt::Key_Percent, 0x2e},
                                    {Qt::Key_AsciiCircum, 0x36},
                                    {Qt::Key_Ampersand, 0x3d},
                                    {Qt::Key_Asterisk, 0x3e},
                                    {Qt::Key_ParenLeft, 0x46},
                                    {Qt::Key_ParenRight, 0x45},
                                    {Qt::Key_QuoteLeft, 0x0e},
                                    {Qt::Key_AsciiTilde, 0x0e},

                                    {Qt::Key_Minus, 0x4e},
                                    {Qt::Key_Underscore, 0x4e},
                                    {Qt::Key_Plus, 0x55},
                                    {Qt::Key_Equal, 0x55},
                                    {Qt::Key_BraceLeft, 0x54},
                                    {Qt::Key_BracketLeft, 0x54},
                                    {Qt::Key_BraceRight, 0x5b},
                                    {Qt::Key_BracketRight, 0x5b},
                                    {Qt::Key_Backslash, 0x5d},
                                    {Qt::Key_Bar, 0x5d},
                                    {Qt::Key_Colon, 0x4c},
                                    {Qt::Key_Semicolon, 0x4c},
                                    {Qt::Key_Apostrophe, 0x52},
                                    {Qt::Key_QuoteDbl, 0x52},

                                    {Qt::Key_Less, 0x41},
                                    {Qt::Key_Comma, 0x41},
                                    {Qt::Key_Greater, 0x49},
                                    {Qt::Key_Period, 0x49},
                                    {Qt::Key_Question, 0x4a},
                                    {Qt::Key_Slash, 0x4a},

                                    {Qt::Key_Q, 0x15},
                                    {Qt::Key_W, 0x1d},
                                    {Qt::Key_E, 0x24},
                                    {Qt::Key_R, 0x2d},
                                    {Qt::Key_T, 0x2c},
                                    {Qt::Key_Y, 0x35},
                                    {Qt::Key_U, 0x3c},
                                    {Qt::Key_I, 0x43},
                                    {Qt::Key_O, 0x44},
                                    {Qt::Key_P, 0x4d},
                                    {Qt::Key_A, 0x1c},
                                    {Qt::Key_S, 0x1b},

                                    {Qt::Key_D, 0x23},
                                    {Qt::Key_F, 0x2b},
                                    {Qt::Key_G, 0x34},
                                    {Qt::Key_H, 0x33},
                                    {Qt::Key_J, 0x3b},
                                    {Qt::Key_K, 0x42},
                                    {Qt::Key_L, 0x4b},
                                    {Qt::Key_Z, 0x1a},
                                    {Qt::Key_X, 0x22},
                                    {Qt::Key_C, 0x21},
                                    {Qt::Key_V, 0x2a},
                                    {Qt::Key_B, 0x32},
                                    {Qt::Key_N, 0x31},
                                    {Qt::Key_M, 0x3a}, //,

                                    {Qt::Key_Tab, 0x0d},
                                    {Qt::Key_Backspace, 0x66},
                                    {Qt::Key_CapsLock, 0x58},
                                    {Qt::Key_Shift, 0x12},
                                    {Qt::Key_Control, 0x14},
                                    {Qt::Key_Alt, 0x11},
                                    {Qt::Key_Return, 0x5a},
                                    {Qt::Key_Space, 0x29},
                                    {Qt::Key_AltGr, 0x12}, // Gr right E0
                                    {Qt::Key_Insert, 0x70},
                                    {Qt::Key_Delete, 0x71},
                                    {Qt::Key_Home, 0x6c},
                                    {Qt::Key_End, 0x69},
                                    {Qt::Key_PageUp, 0x7d},
                                    {Qt::Key_PageDown, 0x7a},
                                    {Qt::Key_Up, 0x75},
                                    {Qt::Key_Down, 0x72},
                                    {Qt::Key_Left, 0x6b},
                                    {Qt::Key_Right, 0x74}

};

auto PS2KeyboardRawComponent::keyMapIndex(const int &keyType) -> int {
  if (auto iter = keyMap.find(keyType); iter != keyMap.end()) {
    return iter.value();
  }
  return -1;
}

PS2KeyboardRawComponent::PS2KeyboardRawComponent(QWidget *parent)
    : AbstractRawComponent(parent) {
  initPorts();
  keyboard_picture_ = &PS2KeyboardRawComponent::keyboardPictureStatic();
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  keyboard_picture_label_ = new QLabel(this);
  keyboard_picture_label_->setPixmap(*keyboard_picture_);
  keyboard_picture_label_->setScaledContents(true);
  keyboard_picture_label_->setSizePolicy(QSizePolicy::Ignored,
                                         QSizePolicy::Ignored);
  layout->addWidget(keyboard_picture_label_);

  setLayout(layout);

  initConnections();
  setFocusPolicy(Qt::StrongFocus);
  // setFocus();
}

PS2KeyboardRawComponent::~PS2KeyboardRawComponent() {}

void PS2KeyboardRawComponent::reset() {
  key_pressed_num_ = 0;
  while (!pclk_to_write_.empty()) {
    pclk_to_write_.pop();
  }
  while (!pdata_to_write_.empty()) {
    pdata_to_write_.pop();
  }

  update();
}

void PS2KeyboardRawComponent::initConnections() {
  // signal slot connect
  connect(this, &PS2KeyboardRawComponent::keyPressed, this,
          &PS2KeyboardRawComponent::keyProcess);
  connect(this, &PS2KeyboardRawComponent::keyReleased, this,
          &PS2KeyboardRawComponent::keyProcess);
}

bool PS2KeyboardRawComponent::checkSpecialKey2(int keyValue) {
  bool flag = (keyValue == 0x71) | (keyValue == 0x6c) | (keyValue == 0x69) |
              (keyValue == 0x7d) | (keyValue == 0x7a) | (keyValue == 0x75) |
              (keyValue == 0x72) | (keyValue == 0x74) | (keyValue == 0x6b) |
              (keyValue == 0x70);
  return flag;
}

void PS2KeyboardRawComponent::pclkEnqueue(int cycle, bool data) {
  for (int i = 0; i < cycle; i++) {
    pclk_to_write_.push(data);
  }
}

void PS2KeyboardRawComponent::pdataEnqueue(int cycle, bool data) {
  for (int i = 0; i < cycle; i++) {
    pdata_to_write_.push(data);
  }
}

void PS2KeyboardRawComponent::pclkWriteDataGenerate(int keyValue) {
  // blank
  pclkEnqueue(8, true);
  // start
  for (int i = 0; i < 11; i++) {
    pclkEnqueue(4, true);
    pclkEnqueue(4, false);
  }
  // blank
  pclkEnqueue(8, true);
}

void PS2KeyboardRawComponent::pdataWriteDataGenerate(int keyValue) {
  int parityCount = 0;
  // blank
  pdataEnqueue(8, true);
  // start bit
  pdataEnqueue(8, false);
  for (int i = 0; i < 8; i++) {
    if (keyValue & (1 << i)) {
      pdataEnqueue(8, true);
      parityCount++;
    } else {
      pdataEnqueue(8, false);
    }
  }
  if (parityCount % 2) {
    pdataEnqueue(8, false);
  } else {
    pdataEnqueue(8, true);
  }
  // stop bit
  pdataEnqueue(8, true);
  // blank
  pdataEnqueue(8, true);
}

void PS2KeyboardRawComponent::keyPressEvent(QKeyEvent *event) {
  emit keyPressed(event->key(), 1);
}

void PS2KeyboardRawComponent::keyReleaseEvent(QKeyEvent *event) {
  emit keyReleased(event->key(), 2);
}

void PS2KeyboardRawComponent::focusInEvent(QFocusEvent *event) {
  item_foucused_ = true;
  keyboard_picture_ = &PS2KeyboardRawComponent::keyboardPictureOnFocus();
  keyboard_picture_label_->setPixmap(*keyboard_picture_);
}

void PS2KeyboardRawComponent::focusOutEvent(QFocusEvent *event) {
  item_foucused_ = false;
  keyboard_picture_ = &PS2KeyboardRawComponent::keyboardPictureStatic();
  keyboard_picture_label_->setPixmap(*keyboard_picture_);
}

void PS2KeyboardRawComponent::keyProcess(int keyType, int state) {
  int info = keyMapIndex(keyType);
  bool specialKeyType2 = checkSpecialKey2(info);
  if (state == 1) {
    // std::cout<< "pressed";
    key_pressed_num_++;

    if (specialKeyType2) {
      pclkWriteDataGenerate(0xe0);
      pdataWriteDataGenerate(0xe0);
      pclkWriteDataGenerate(info);
      pdataWriteDataGenerate(info);
    } else {
      pclkWriteDataGenerate(info);
      pdataWriteDataGenerate(info);
    }
  } else if (state == 2) {
    // std::cout<< "released";
    key_pressed_num_--;
    if (specialKeyType2) {
      pclkWriteDataGenerate(0xe0);
      pdataWriteDataGenerate(0xe0);
      pclkWriteDataGenerate(0xf0);
      pdataWriteDataGenerate(0xf0);
      pclkWriteDataGenerate(info);
      pdataWriteDataGenerate(info);
    } else {
      pclkWriteDataGenerate(0xf0);
      pdataWriteDataGenerate(0xf0);
      pclkWriteDataGenerate(info);
      pdataWriteDataGenerate(info);
    }
  }

  if (key_pressed_num_ != 0) {
    keyboard_picture_ = &PS2KeyboardRawComponent::keyboardPictureInUse();
  } else if (item_foucused_) {
    keyboard_picture_ = &PS2KeyboardRawComponent::keyboardPictureOnFocus();
  } else {
    keyboard_picture_ = &PS2KeyboardRawComponent::keyboardPictureStatic();
  }

  keyboard_picture_label_->setPixmap(*keyboard_picture_);
}

uint64_t PS2KeyboardRawComponent::getWriteData() const {
  uint64_t data = 0;
  bool pclk = true;
  bool pdata = true;
  if (!pclk_to_write_.empty()) {
    pclk = pclk_to_write_.front();
    pclk_to_write_.pop();
    // std::cout<<"pclk: "<<pclk<<std::endl;
  } else {
    pclk = true;
  }
  if (!pdata_to_write_.empty()) {
    pdata = pdata_to_write_.front();
    pdata_to_write_.pop();
    // std::cout<<"pdata: "<<pdata<<std::endl;
  } else {
    pdata = true;
  }

  // PCLK :
  data |= pclk << input_ports_[0].pin_index;
  // PDATA :
  data |= pdata << input_ports_[1].pin_index;

  return data;
}

void PS2KeyboardRawComponent::processReadData(QQueue<uint64_t> &read_queue) {}

void PS2KeyboardRawComponent::paintEvent(QPaintEvent *event) {}

void PS2KeyboardRawComponent::initPorts() {
  appendPort(input_ports_, "PCLK", ports::PortType::Input);
  appendPort(input_ports_, "PDATA", ports::PortType::Input);
}

COMPONENT_CLASS_DEFINITION(PS2Keyboard, 3, 3)

void PS2KeyboardComponent::onSettingsBtnClicked() {
  auto settings_dialog = new ComponentSettingsDialog(this, this);
  settings_dialog->exec();
  delete settings_dialog;
}