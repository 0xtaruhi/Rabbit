#include <QImage>
#include <QLayout>
#include <QPainter>
#include <QThread>
#include <QTimer>
#include <iostream>

#include "Common.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/GraphicLCDComponent.h"
#include "qboxlayout.h"
#include "qnamespace.h"

using namespace rabbit_App;
using namespace rabbit_App::component;

GraphicLCDRawComponent::GraphicLCDRawComponent(QWidget *parent)
    : AbstractRawComponent(parent) {
  initPorts();
  mem_.resize(kMemSize);
  mem_.fill(kBackground);
  image_ = new QImage(kLCDWidth * kLCDUnit, kLCDHeight * kLCDUnit,
                      QImage::Format_RGB32);
  image_->fill(kBackgroundColor);
}

GraphicLCDRawComponent::~GraphicLCDRawComponent() {}

void GraphicLCDRawComponent::reset() {
  x_addr_ = 0;
  y_addr_ = 0;
  mem_addr_ = 0;
  mem_.fill(kBackground);
  image_->fill(kBackgroundColor);
  this->update();
}

void GraphicLCDRawComponent::processReadData(QQueue<uint64_t> &read_queue) {
  for (auto data : read_queue) {
    db_ = 0;
    for (int i = 0; i < kDataPortWidth; i++) {
      db_ += ((data >> (output_ports_[i].pin_index - 1)) & 0x1) << i;
    }
    di_ = (data >> (output_ports_[kDataPortWidth].pin_index - 1)) & 0x1;
    rw_ = (data >> (output_ports_[kDataPortWidth + 1].pin_index - 1)) & 0x1;
    en_ = (data >> (output_ports_[kDataPortWidth + 2].pin_index - 1)) & 0x1;
    cs1_ = (data >> (output_ports_[kDataPortWidth + 3].pin_index - 1)) & 0x1;
    cs2_ = (data >> (output_ports_[kDataPortWidth + 4].pin_index - 1)) & 0x1;
    rst_ = (data >> (output_ports_[kDataPortWidth + 5].pin_index - 1)) & 0x1;
    if (rst_) {
      reset();
    } else if (!en_) {
      di_ ? processData() : processCommand();
    }
  }
}

uint64_t GraphicLCDRawComponent::getWriteData() const { return 0; }

void GraphicLCDRawComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  QRect rect = this->rect();
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setBrush(kBackgroundColor);
  painter.drawRoundedRect(
      rect.x() - kPadding, rect.y() - kPadding, rect.width() + 2 * kPadding,
      rect.height() + 2 * kPadding, kRoundRadius, kRoundRadius);
  auto start_x =
      (rect.width() - 2 * kPadding - kLCDWidth * kLCDUnit) / 2 + kPadding;
  auto start_y =
      (rect.height() - 2 * kPadding - kLCDHeight * kLCDUnit) / 2 + kPadding;
  painter.setWindow(start_x, start_y, kLCDWidth * kLCDUnit,
                    kLCDHeight * kLCDUnit);
  painter.drawImage(start_x, start_y, *image_);
}

void GraphicLCDRawComponent::initPorts() {
  for (int i = 0; i < kDataPortWidth; i++) {
    appendPort(output_ports_, QString("DB%1").arg(i), ports::PortType::Output);
  }
  appendPort(output_ports_, "DI", ports::PortType::Output);
  appendPort(output_ports_, "RW", ports::PortType::Output);
  appendPort(output_ports_, "EN", ports::PortType::Output);
  appendPort(output_ports_, "CS1", ports::PortType::Output);
  appendPort(output_ports_, "CS2", ports::PortType::Output);
  appendPort(output_ports_, "RST", ports::PortType::Output);
}

void GraphicLCDRawComponent::processData() {
  for (int i = 0; i < kDataPortWidth; i++) {
    mem_[mem_addr_ * kDataPortWidth + i] =
        ((db_ >> (kDataPortWidth - 1 - i)) & 0x1);
  }
  mem_addr_++;
  if (mem_addr_ == kMaxYAxis)
    mem_addr_ = 0;
}

void GraphicLCDRawComponent::processCommand() {
  switch ((db_ >> 6) & 0x3) {
  case 0x00: {
    if (((db_ >> 1) & 0b11111) == 0b11111) {
      switch (db_ & 0x1) {
      case 0: {
        writeToImage();
        break;
      }
      case 1: {
        image_->fill(kBackgroundColor);
        break;
      }
      }
    }
    break;
  }
  case 0x01:
    mem_addr_ = db_ & 0b111111;
    break;
  case 0x02: {
    if (cs1_ ^ cs2_) {
      x_addr_ = (db_ & 0b111) + (cs1_ ? 0 : kMaxXAxis);
    }
    break;
  }
  case 0x03:
    y_addr_ = db_ & 0b111111;
    break;
  default:
    break;
  }
}

void GraphicLCDRawComponent::writeToImage() {
  int mem_pos = (kMaxYAxis - y_addr_) * kMemWidth;
  for (int row = 0; row < kLCDHeight; row++) {
    for (int col = x_addr_ * kMemWidth; col < x_addr_ * kMemWidth + kMemWidth;
         col++) {
      if (mem_pos == kMemSize)
        mem_pos = 0;
      for (int image_row = 0; image_row < kLCDUnit; image_row++) {
        for (int image_col = 0; image_col < kLCDUnit; image_col++) {
          auto line = (QRgb *)image_->scanLine(row * kLCDUnit + image_row);
          line += col * kLCDUnit + image_col;
          *line = mem_[mem_pos] == kShow ? kShowColor.rgb()
                                         : kBackgroundColor.rgb();
        }
      }
      mem_pos++;
    }
  }
}

GraphicLCDComponent::GraphicLCDComponent(QWidget *parent)
    : AbstractComponent(parent) {
  raw_component_ = new GraphicLCDRawComponent(this);
  layout()->addWidget(raw_component_);
}

GraphicLCDComponent::~GraphicLCDComponent() {}

QSize GraphicLCDComponent::gridOccupation() const noexcept {
  return QSize(9, 5);
}

void GraphicLCDComponent::contextMenuEvent(QContextMenuEvent *event) {
  AbstractComponent::contextMenuEvent(event);
}

void GraphicLCDComponent::onSettingsBtnClicked() {
  ComponentSettingsDialog *dialog = new ComponentSettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}

// void GraphicLCDComponent::setSettingsDialog() {
//   settings_dialog_ = new ComponentSettingsDialog(this, this);
// }