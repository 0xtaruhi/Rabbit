#pragma once
#ifndef SEGMENT_DISPLAY_COMPONENT_H
#define SEGMENT_DISPLAY_COMPONENT_H

#include <QColor>
#include <QLabel>
#include <QMap>
#include <QVector>
#include <QWidget>

#include "ComponentSettingsDialog.h"
#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(SegmentDisplay)
COMPONENT_CLASS_DECLARATION(FourDigitSegmentDisplay)

class SingleSegmentDisplay : public QWidget {
  Q_OBJECT

public:
  static constexpr int A = 0;
  static constexpr int B = 1;
  static constexpr int C = 2;
  static constexpr int D = 3;
  static constexpr int E = 4;
  static constexpr int F = 5;
  static constexpr int G = 6;
  static constexpr int DP = 7;
  static constexpr int kSegmentCount = 8;

public:
  SingleSegmentDisplay(QWidget *parent = nullptr);
  virtual ~SingleSegmentDisplay();

  void setSegment(const int segment_section, const bool value);
  void setSegments(const QVector<bool> &segments) { segments_ = segments; }
  QVector<bool> &segments() { return segments_; }
  void setEn(const bool en);
  void reset();

  void setVisionPersistence(int vision_persistence) noexcept {
    vision_persistence_ = vision_persistence;
  }

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  void paintOneSegment(QPainter &painter, const QList<QPoint> &points,
                       const int segment_section);
  void paintDP(QPainter &painter);

private:
  int vision_persistence_ = 0;
  bool on_persistence_[kSegmentCount] = {false, false, false, false,
                                         false, false, false, false};
  int counter_[kSegmentCount] = {0, 0, 0, 0, 0, 0, 0, 0};

  QVector<bool> segments_;
  bool en_;

  QColor brushColor;
  QColor penColor;
  QColor brushColorOff;
  QColor penColorOff;
  int penWidth;

  static const QList<QPoint> a_points;
  static const QList<QPoint> b_points;
  static const QList<QPoint> c_points;
  static const QList<QPoint> d_points;
  static const QList<QPoint> e_points;
  static const QList<QPoint> f_points;
  static const QList<QPoint> g_points;
  static const QList<QPoint> dp_points;

}; // class SingleSegmentDisplay

class SegmentDisplayRawComponent : public AbstractRawComponent {
  Q_OBJECT

public:
  SegmentDisplayRawComponent(QWidget *parent = nullptr);
  ~SegmentDisplayRawComponent();

  void reset() override;
  // void setPortsIndex() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

protected:
  void paintEvent(QPaintEvent *event) override;

  void initPorts() override;

private:
  SingleSegmentDisplay *single_segment_display_;

}; // class SegmentDisplayRawComponent

class FourDigitSegmentDisplayRawComponent : public AbstractRawComponent {
  Q_OBJECT

public:
  static constexpr int kDigitCount = 4;

public:
  FourDigitSegmentDisplayRawComponent(QWidget *parent = nullptr);
  ~FourDigitSegmentDisplayRawComponent();

  void reset() override;
  // void setPortsIndex() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

  void setVisionPersistence(int vision_persistence) override;

protected:
  void paintEvent(QPaintEvent *event) override;

  void initPorts() override;

private:
  QVector<SingleSegmentDisplay *> multi_segments_;
  QVector<bool> segment_select_;

}; // class FourDigitSegmentDisplayRawComponent

class SegmentDisplaySettingsDialog : public ActiveModeSettingsDialog,
                                     public VisionPersistenceSettingsDialog {
  Q_OBJECT

public:
  SegmentDisplaySettingsDialog(AbstractComponent *component,
                               QWidget *parent = nullptr);
  virtual ~SegmentDisplaySettingsDialog();

protected:
  void acceptDerivedClassSettings() override;

}; // class SegmentDisplaySettingsDialog

} // namespace rabbit_App::component

#endif // SEGMENT_DISPLAY_COMPONENT_H