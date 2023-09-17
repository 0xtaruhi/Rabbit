#include <QEvent>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>

#include "Common.h"
#include "Components/AbstractComponent.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/RotaryButtonComponent.h"

using namespace rabbit_App;
using namespace rabbit_App::component;

const QColor RotaryButtonRawComponent::kBorderColor = QColor(18, 150, 219);
const QColor RotaryButtonRawComponent::kBallColor = QColor(244, 234, 41);
const QColor RotaryButtonRawComponent::kInnerColor = QColor(223, 234, 245);
const QColor RotaryButtonRawComponent::kScaleColor = Qt::black;

RotaryButtonRawComponent::RotaryButtonRawComponent(QWidget *parent)
    : AbstractRawComponent(parent) {
  initPorts();
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  reset();
}

RotaryButtonRawComponent::~RotaryButtonRawComponent() {}

void RotaryButtonRawComponent::reset() {
  scale_ = 0;
  ball_angle_ = kDefaultAngle;
  update();
}

void RotaryButtonRawComponent::processReadData(QQueue<uint64_t> &read_queue) {}

uint64_t RotaryButtonRawComponent::getWriteData() const {
  uint64_t data = 0x7FF;
  if (scale_ != 0) {
    // auto port = input_ports_[scale_ - 1].pin_index;
    auto index = input_ports_[scale_ - 1].pin_index;
    data = data ^ (1 << index);
  }
  return data;
}

bool RotaryButtonRawComponent::isInInnerCircle(const QPointF &pos) const {
  return (pos - kCenter).manhattanLength() < kScaleDistance;
}

void RotaryButtonRawComponent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setBrush(kInnerColor);
  painter.setPen(color::ruri);
  auto rect = this->rect();
  center_ = rect.center();
  auto real_ball_pos_ = center_ + findPosition(ball_angle_) - kCenter;
  painter.setBrush(kBorderColor);
  painter.drawEllipse(center_.x() - kBorderRadius, center_.y() - kBorderRadius,
                      2 * kBorderRadius, 2 * kBorderRadius);
  painter.setBrush(kInnerColor);
  painter.drawEllipse(center_.x() - kInnerRadius, center_.y() - kInnerRadius,
                      2 * kInnerRadius, 2 * kInnerRadius);
  painter.setBrush(kBallColor);
  painter.drawEllipse(real_ball_pos_.x() - kBallRadius,
                      real_ball_pos_.y() - kBallRadius, 2 * kBallRadius,
                      2 * kBallRadius);

  painter.setPen(kScaleColor);
  for (int i = 1; i <= kScaleCount; ++i) {
    auto angle = i * 2 * M_PI / kScaleCount + kDefaultAngle;
    auto centre_pos = findPosition(angle, kScaleDistance) + center_;
    auto text = QString::number(i);
    // set text font
    QFont font;
    font.setPointSize(8);
    painter.setFont(font);
    painter.drawText(centre_pos.x() - kScaleWidth / 2,
                     centre_pos.y() - kScaleWidth / 2, kScaleWidth, kScaleWidth,
                     Qt::AlignCenter, text);
  }
}

void RotaryButtonRawComponent::mousePressEvent(QMouseEvent *event) {
  auto destination = event->pos() - center_;
  if (!isInInnerCircle(destination)) {
    return;
  } else {
    auto dest_angle = findAngle(destination);
    auto angle_unit = 2 * M_PI / kScaleCount;
    scale_ = static_cast<int>((dest_angle + angle_unit / 2) / angle_unit) + 3;
    scale_ = scale_ % kScaleCount;
    ball_angle_ = scale_ * angle_unit + kDefaultAngle;
    // qDebug() << "scale: " << scale_ << " angle: " << ball_angle_;
    // update();
  }
}

void RotaryButtonRawComponent::initPorts() {
  for (int i = 0; i < kScaleCount - 1; ++i) {
    appendPort(input_ports_, QString("RPB%1").arg(i), ports::PortType::Input);
  }
}

double RotaryButtonRawComponent::findAngle(const Fvector &vector) const {
  // compare to the x axis, the angle is in [0, 2 * M_PI)
  double angle = std::atan2(vector.y(), vector.x());
  if (angle < 0) {
    angle += 2 * M_PI;
  }
  return angle;
}

double RotaryButtonRawComponent::findAngle(int scale) const {
  return scale * 2 * M_PI / kScaleCount + kDefaultAngle;
}

QPointF RotaryButtonRawComponent::findPosition(const float angle,
                                               const float distance) const {
  return QPointF(distance * cos(angle), distance * sin(angle));
}

COMPONENT_CLASS_DEFINITION(RotaryButton, 3, 3)

void RotaryButtonComponent::onSettingsBtnClicked() {
  ComponentSettingsDialog *dialog = new ComponentSettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}