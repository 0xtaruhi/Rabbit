#pragma once
#ifndef ROTARY_BUTTON_COMPONENT_H
#define ROTARY_BUTTON_COMPONENT_H

#include <QPainter>
#include <QThread>
#include <QTimer>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include "ThreadTimer.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(RotaryButton)

/// @brief RotaryButtonRawComponent class
/// This class implements the Rotary Button component.
class RotaryButtonRawComponent : public AbstractRawComponent {
  Q_OBJECT

  typedef QPointF Fvector;

public:
  // constance
  constexpr static float kBorderRadius = 35.0f;
  constexpr static float kInnerRadius = 32.0f;
  constexpr static float kDistance = 24.0f;
  constexpr static float kBallRadius = 6.0f;
  constexpr static int kScaleCount = 12;
  constexpr static float kScaleDistance = 43.0f;
  constexpr static float kScaleWidth = 12.0f;
  constexpr static float kDefaultAngle = (float)(-M_PI / 2);

  constexpr static QPoint kCenter = QPoint(0, 0);

  const static QColor kBorderColor;
  const static QColor kBallColor;
  const static QColor kInnerColor;
  const static QColor kScaleColor;

  RotaryButtonRawComponent(QWidget *parent = nullptr);
  virtual ~RotaryButtonRawComponent();

  void reset() override;
  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

  /// @brief Whether the click position is in the inner circle.
  bool isInInnerCircle(const QPointF &pos) const;

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

  void initPorts() override;

  void onTimerTimeout();

private:
  double findAngle(const Fvector &vector) const;
  double findAngle(int scale) const;
  QPointF findPosition(const float angle,
                       const float distance = kDistance) const;

  QPointF center_;
  float ball_angle_;
  int scale_; // 0 - 11

}; // class RotaryButtonRawComponent

} // namespace rabbit_App::component

#endif // ROTARY_BUTTON_COMPONENT_H