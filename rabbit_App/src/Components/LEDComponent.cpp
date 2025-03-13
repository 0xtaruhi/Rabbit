#include <QGradient>
#include <QLayout>
#include <QPainter>
#include <QRadialGradient>
#include <QSize>
#include <QThreadPool>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/LEDComponent.h"
#include "qpainter.h"

using namespace rabbit_App::component;

LEDRawComponent::LEDRawComponent(QWidget *parent)
    : AbstractRawComponent(parent) {
  setSupportedColors({{tr("Red"), Qt::red},
                      {tr("Green"), Qt::green},
                      {tr("Blue"), Qt::blue},
                      {tr("Yellow"), Qt::yellow},
                      {tr("Black"), Qt::black},
                      {tr("White"), Qt::white}});
  appendColor(tr("LED color"), Qt::red);

  initPorts();

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  led_picture_ = new QLabel(this);
  led_picture_->setPixmap(QPixmap(":/icons/icons/icons8-LED-94.png"));

  led_picture_->setScaledContents(true);
  led_picture_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  auto layout = new QHBoxLayout(this);
  layout->addWidget(led_picture_);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}

LEDRawComponent::~LEDRawComponent() {}

void LEDRawComponent::reset() {
  // is_on_ = false;
  level_ = 0.0f;
  update();
}

void LEDRawComponent::processReadData(QQueue<uint64_t> &read_queue) {
  if (read_queue.isEmpty()) {
    return;
  }
  std::array<int, 64> value_count = {0};
  for (auto value : read_queue) {
    for (auto i = 0; i != 64; ++i) {
      if ((value >> i) & 0x1) {
        ++value_count[i];
      }
    }
  }
  auto queue_size = read_queue.size();
  auto pin_index = output_ports_[0].pin_index - 1;
  auto level = static_cast<float>(value_count[pin_index]) / queue_size;
  onSetLevel(is_low_active_ ? 1.0f - level : level);
}
uint64_t LEDRawComponent::getWriteData() const { return 0; }

void LEDRawComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  QRadialGradient gradient(this->rect().center(), 20);
  // gradient.setColorAt(0, is_on_ ? Qt::red : Qt::transparent);
  gradient.setColorAt(0, component_colors_[tr("LED color")]);
  gradient.setColorAt(1, Qt::transparent);
  painter.setOpacity(level_);
  painter.setBrush(gradient);
  painter.setPen(Qt::transparent);
  painter.drawEllipse(this->rect());
}

void LEDRawComponent::initPorts() {
  appendPort(output_ports_, "LED", ports::PortType::Output);
}

// void LEDRawComponent::onSwitchLED(bool is_on) {
//   is_on_ = is_on;
// }

// LEDComponent::LEDComponent(QWidget *parent) : AbstractComponent(parent) {
//   raw_component_ = new LEDRawComponent(this);
//   layout()->addWidget(raw_component_);
// }

// LEDComponent::~LEDComponent() {}

// QSize LEDComponent::gridOccupation() const noexcept { return QSize(2, 2); }

// void LEDComponent::contextMenuEvent(QContextMenuEvent *event) {
//   AbstractComponent::contextMenuEvent(event);
// }

COMPONENT_CLASS_DEFINITION(LED, 2, 2)

void LEDComponent::onSettingsBtnClicked() {
  auto settings_dialog = new LEDSettingsDialog(this, this);
  settings_dialog->exec();
  delete settings_dialog;
}
// void LEDComponent::setSettingsDialog() {
//   settings_dialog_ = new LEDSettingsDialog(this, this);
// }
