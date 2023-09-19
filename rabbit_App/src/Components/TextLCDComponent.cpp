#include <QLayout>
#include <QPainter>
#include <QPalette>

#include "Common.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/TextLCDComponent.h"

using namespace rabbit_App;
using namespace rabbit_App::component;

CharBlock::CharBlock(QWidget *parent) : QWidget(parent) {
  this->setFixedSize(15, 20);
}

CharBlock::~CharBlock() {}

void CharBlock::setCharCode(const quint8 &char_code) {
  char_code_ = char_code;
  // this->update();
}

void CharBlock::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  // painter.setBrush(Qt::transparent);
  painter.setPen(color::ruri);
  painter.setFont(QFont("Arial", 13));
  painter.drawText(0, 0, 20, 20, Qt::AlignCenter,
                   QChar(uchar(char_code_ + 0x20)));
}

TextLCDRawComponent::TextLCDRawComponent(QWidget *parent)
    : AbstractRawComponent(parent) {
  initPorts();
  auto layout = new QVBoxLayout(this);
  auto upper_layout = new QHBoxLayout();
  auto lower_layout = new QHBoxLayout();

  {
    int i = 0;
    CharBlock *char_block;
    while (i != 16) {
      char_block = new CharBlock(this);
      char_blocks_.push_back(char_block);
      upper_layout->addWidget(char_block);
      ++i;
    }

    while (i != 32) {
      char_block = new CharBlock(this);
      char_blocks_.push_back(char_block);
      lower_layout->addWidget(char_block);
      ++i;
    }
  }

  layout->addLayout(upper_layout);
  layout->addLayout(lower_layout);
}

TextLCDRawComponent::~TextLCDRawComponent() {}

void TextLCDRawComponent::reset() {
  for (auto &char_block : char_blocks_) {
    char_block->setCharCode(0x00);
  }
  cur_pos_ = 0;
  update();
}

void TextLCDRawComponent::processReadData(QQueue<uint64_t> &read_queue) {
  for (auto value : read_queue) {
    auto en_index = output_ports_[0].pin_index - 1;
    auto en = (value >> en_index) & 0x1;
    if (en == 0) {
      uint8_t char_code = 0x00;
      for (int i = 0; i < 8; i++) {
        auto db_index = output_ports_[i + 1].pin_index - 1;
        char_code |= ((value >> db_index) & 0x1) << i;
      }
      setChar(static_cast<quint8>(char_code));
    }
  }
}

uint64_t TextLCDRawComponent::getWriteData() const { return 0; }

void TextLCDRawComponent::setChar(const quint8 &char_code,
                                  const quint8 &char_pos) {
  char_blocks_[char_pos]->setCharCode(char_code);
}

void TextLCDRawComponent::setChar(const quint8 &char_code) {
  char_blocks_[cur_pos_]->setCharCode(char_code);
  moveCursorNext();
}

void TextLCDRawComponent::moveCursorNext() { cur_pos_ = (cur_pos_ + 1) % 32; }

void TextLCDRawComponent::setCursorPos(const int &pos) {
  assert(pos >= 0 && pos < 32);
  cur_pos_ = pos;
}

void TextLCDRawComponent::moveCursorHome() { cur_pos_ = 0; }

void TextLCDRawComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(Qt::gray);
  painter.setBrush(color::wasurenagusa);
  painter.drawRect(rect());
}

void TextLCDRawComponent::initPorts() {
  appendPort(output_ports_, "EN", ports::PortType::Output);
  for (int i = 0; i < 8; i++) {
    appendPort(output_ports_, QString("DB%1").arg(i), ports::PortType::Output);
  }
}

// TextLCDComponent::TextLCDComponent(QWidget *parent)
//     : AbstractComponent(parent) {
//   raw_component_ = new TextLCDRawComponent(this);
//   layout()->addWidget(raw_component_);
// }

// TextLCDComponent::~TextLCDComponent() {}

// QSize TextLCDComponent::gridOccupation() const noexcept { return QSize(8, 2);
// }

// void TextLCDComponent::contextMenuEvent(QContextMenuEvent *event) {
//   AbstractComponent::contextMenuEvent(event);
// }

COMPONENT_CLASS_DEFINITION(TextLCD, 8, 2)

void TextLCDComponent::onSettingsBtnClicked() {
  AbstractComponent::onSettingsBtnClicked();
}