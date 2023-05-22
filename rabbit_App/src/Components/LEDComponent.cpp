#include <QLayout>
#include <QPainter>
#include <QSize>

#include "Components/Components.h"

using namespace rabbit_App::component;

LEDRawComponent::LEDRawComponent(QWidget *parent) : QWidget(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  led_picture_ = new QLabel(this);
  led_picture_->setPixmap(
      QPixmap(":/icons/icons/components/icons8-light-94.png"));

  led_picture_->setScaledContents(true);
  led_picture_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  auto layout = new QHBoxLayout(this);
  layout->addWidget(led_picture_);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}

LEDRawComponent::~LEDRawComponent() {}

void LEDRawComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  // painter.setRenderHint(QPainter::Antialiasing);
  // painter.setPen(Qt::NoPen);
  // painter.setBrush(color_);
  // painter.drawRect(0, 0, width(), height());
}

LEDComponent::LEDComponent(QWidget *parent) : AbstractOutputComponent(parent) {
  raw_component_ = new LEDRawComponent(this);
  layout()->addWidget(raw_component_);
}

LEDComponent::~LEDComponent() {}

QSize LEDComponent::gridOccupation() const noexcept { return QSize(1, 1); }
