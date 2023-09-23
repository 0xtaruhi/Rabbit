#include <QBoxLayout>
#include <QEvent>
#include <QLayout>
#include <QPainter>
#include <QPalette>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QValidator>

#include "Common.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/LEDMatrixComponent.h"

using namespace rabbit_App;
using namespace rabbit_App::component;

SingleLED::SingleLED(QWidget *parent) : QWidget(parent) {
  state_ = false;
  level_ = 0.0;
  radius_ = kDefaultRadius;
  size_ = kDefaultSize;
  counter_ = 0;
  setFixedSize(size_, size_);
}

SingleLED::SingleLED(float radius, QWidget *parent) : QWidget(parent) {
  state_ = false;
  level_ = 0.0;
  radius_ = radius;
  size_ = radius_ * 2 + 2;
  counter_ = 0;
  setFixedSize(size_, size_);
}

SingleLED::~SingleLED() {}

void SingleLED::setLEDState(bool state) {
  if (state_ && !state && vision_persistence_ != 0) {
    on_persistence_ = true;
    counter_ = 0;
  }
  state_ = state;
}

void SingleLED::setLEDLevel(float level) {
  level_ = level;
  if (level_ > 0.0) {
    state_ = true;
  } else {
    state_ = false;
  }
  // update();
}

void SingleLED::setVisionPersistence(uint vision_persistence) {
  vision_persistence_ = vision_persistence;
  // timer_->setInterval(vision_persistence_);
}

void SingleLED::paintEvent(QPaintEvent *event) {
  if (on_persistence_) {
    counter_++;
    auto max = vision_persistence_ * 60 / 3000;
    if (counter_ > max) {
      on_persistence_ = false;
      counter_ = 0;
    }
  }
  bool actual_state = state_ || on_persistence_;
  QPainter painter(this);
  if (!actual_state) {
    painter.setBrush(kOffColor);
    painter.setPen(kOffColor);
  } else {
    painter.setBrush(kOnColor);
    painter.setPen(kOnColor);
    // qDebug() << "paintEvent called with SingleLED on.";
  }
  painter.drawEllipse(1, 1, radius_ * 2, radius_ * 2);
}

void SingleLED::reset() {
  state_ = false;
  counter_ = 0;
  on_persistence_ = false;
  level_ = 0.0;
  // update();
}

LED4x4MatrixRawComponent::LED4x4MatrixRawComponent(QWidget *parent, bool is_4x4)
    : AbstractRawComponent(parent) {
  // DO NOT initialize ports or initialize singleLED here.
  // They will be called in the constructor of derived class.
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  if (is_4x4) {
    row_num_ = 4;
    column_num_ = 4;
    initPorts();
    initLayout();
    updateAllLEDs();
  }
}

LED4x4MatrixRawComponent::~LED4x4MatrixRawComponent() {}

void LED4x4MatrixRawComponent::reset() {
  for (auto led : leds_vec_) {
    led->reset();
  }
  update();
}

void LED4x4MatrixRawComponent::processReadData(QQueue<uint64_t> &read_queue) {
  for (auto data : read_queue) {
    // qDebug() << "Matrix read data: " << data;
    for (int row = 0; row != row_num_; ++row) {
      auto row_pin_index = output_ports_[row].pin_index - 1;
      bool row_data = (data >> row_pin_index) & 0x1;
      for (int column = 0; column != column_num_; ++column) {
        auto led = leds_vec_[row * column_num_ + column];
        auto column_pin_index = output_ports_[column + row_num_].pin_index - 1;
        bool column_data = (data >> column_pin_index) & 0x1;
        auto state = is_low_active_ ? (!row_data && !column_data)
                                    : (row_data && column_data);
        led->setLEDState(state);
      }
    }
  }
}

uint64_t LED4x4MatrixRawComponent::getWriteData() const { return 0; }

void LED4x4MatrixRawComponent::updateAllLEDs() {
  for (auto led : leds_vec_) {
    led->update();
  }
}

void LED4x4MatrixRawComponent::setVisionPersistence(int vision_persistence) {
  vision_persistence_ = vision_persistence;
  for (auto led : leds_vec_) {
    led->setVisionPersistence(vision_persistence_);
  }
}

void LED4x4MatrixRawComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setBrush(kBackGroundColor);
  painter.setPen(color::ruri);
  auto rect = this->rect();
  int shrink = 2;
  rect.setRect(rect.x() + shrink, rect.y() + shrink, rect.width() - 2 * shrink,
               rect.height() - 2 * shrink);
  painter.drawRoundedRect(rect, 10, 10);
}

void LED4x4MatrixRawComponent::initPorts() {
  // DO NOT use this function until row_num_ and column_num_ are set.
  for (int i = 0; i < row_num_; i++) {
    appendPort(output_ports_, QString("ROW%1").arg(i), ports::PortType::Output);
  }
  for (int i = 0; i < column_num_; i++) {
    appendPort(output_ports_, QString("COL%1").arg(i), ports::PortType::Output);
  }
  // ports_index_.resize(row_num_ + column_num_);
}

void LED4x4MatrixRawComponent::initLayout(float single_led_radius) {
  // DO NOT use this function until row_num_ and column_num_ are set.
  auto grid_layout = new QGridLayout(this);
  grid_layout->setContentsMargins(9, 6, 9, 6);
  grid_layout->setSpacing(0);
  for (int i = 0; i != row_num_; ++i) {
    for (int j = 0; j != column_num_; ++j) {
      auto led = new SingleLED(single_led_radius, this);
      led->setVisionPersistence(vision_persistence_);
      grid_layout->addWidget(led, i, j, Qt::AlignCenter);
      leds_vec_.push_back(led);
    }
  }
}

// LED4x4MatrixComponent::LED4x4MatrixComponent(QWidget *parent)
//     : AbstractComponent(parent) {
//   raw_component_ = new LED4x4MatrixRawComponent(this);
//   layout()->addWidget(raw_component_);
// }

// LED4x4MatrixComponent::~LED4x4MatrixComponent() {}

// QSize LED4x4MatrixComponent::gridOccupation() const noexcept {
//   return QSize(3, 3);
// }

// void LED4x4MatrixComponent::contextMenuEvent(QContextMenuEvent *event) {
//   AbstractComponent::contextMenuEvent(event);
// }

LED8x8MatrixRawComponent::LED8x8MatrixRawComponent(QWidget *parent)
    : LED4x4MatrixRawComponent(parent, false) {
  setColumnNum(8);
  setRowNum(8);
  initPorts();
  initLayout();
  updateAllLEDs();
}

LED8x8MatrixRawComponent::~LED8x8MatrixRawComponent() {}

// LED8x8MatrixComponent::LED8x8MatrixComponent(QWidget *parent)
//     : AbstractComponent(parent) {
//   raw_component_ = new LED8x8MatrixRawComponent(this);
//   layout()->addWidget(raw_component_);
// }

// LED8x8MatrixComponent::~LED8x8MatrixComponent() {}

// QSize LED8x8MatrixComponent::gridOccupation() const noexcept {
//   return QSize(6, 6);
// }

// void LED8x8MatrixComponent::contextMenuEvent(QContextMenuEvent *event) {
//   AbstractComponent::contextMenuEvent(event);
// }

LED16x16MatrixRawComponent::LED16x16MatrixRawComponent(QWidget *parent)
    : LED4x4MatrixRawComponent(parent, false) {
  setColumnNum(16);
  setRowNum(16);
  initPorts();
  initLayout(8.0);
  updateAllLEDs();
}

LED16x16MatrixRawComponent::~LED16x16MatrixRawComponent() {}

// LED16x16MatrixComponent::LED16x16MatrixComponent(QWidget *parent)
//     : AbstractComponent(parent) {
//   raw_component_ = new LED16x16MatrixRawComponent(this);
//   layout()->addWidget(raw_component_);
// }

// LED16x16MatrixComponent::~LED16x16MatrixComponent() {}

// QSize LED16x16MatrixComponent::gridOccupation() const noexcept {
//   return QSize(8, 8);
// }

// void LED16x16MatrixComponent::contextMenuEvent(QContextMenuEvent *event) {
//   AbstractComponent::contextMenuEvent(event);
// }
COMPONENT_CLASS_DEFINITION(LED4x4Matrix, 3, 3)
COMPONENT_CLASS_DEFINITION(LED8x8Matrix, 6, 6)
COMPONENT_CLASS_DEFINITION(LED16x16Matrix, 8, 8)

void LED4x4MatrixComponent::onSettingsBtnClicked() {
  LEDMatrixSettingsDialog *dialog = new LEDMatrixSettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}
void LED8x8MatrixComponent::onSettingsBtnClicked() {
  LEDMatrixSettingsDialog *dialog = new LEDMatrixSettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}
void LED16x16MatrixComponent::onSettingsBtnClicked() {
  LEDMatrixSettingsDialog *dialog = new LEDMatrixSettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}

LEDMatrixSettingsDialog::LEDMatrixSettingsDialog(AbstractComponent *component,
                                                 QWidget *parent)
    : ComponentSettingsDialog(component, parent),
      ActiveModeSettingsDialog(component, parent),
      VisionPersistenceSettingsDialog(component, parent) {}

LEDMatrixSettingsDialog::~LEDMatrixSettingsDialog() {}

void LEDMatrixSettingsDialog::acceptDerivedClassSettings() {
  ActiveModeSettingsDialog::acceptDerivedClassSettings();
  VisionPersistenceSettingsDialog::acceptDerivedClassSettings();
}