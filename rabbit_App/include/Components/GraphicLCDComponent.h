#pragma once
#ifndef GRAPHIC_LCD_COMPONENT_H
#define GRAPHIC_LCD_COMPONENT_H

#include <QBitArray>
#include <QImage>
#include <QObject>
#include <QVector>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(GraphicLCD)

/// @brief GraphicLCDRawComponent class
/// This class implements the Graphic LCD component.
class GraphicLCDRawComponent : public AbstractRawComponent {
  Q_OBJECT

public:
  // constance
  static constexpr int kLCDWidth = 128;
  static constexpr int kLCDHeight = 64;
  static constexpr int kLCDSize = kLCDWidth * kLCDHeight;
  static constexpr int kMaxYAxis = 64;
  static constexpr int kMaxXAxis = 8;
  static constexpr int kLeftScreen = 0;
  static constexpr int kRightScreen = 1;
  static constexpr int kMemWidth = 8;
  static constexpr int kMemHeight = 64;
  static constexpr int kMemSize = kMemWidth * kMemHeight;
  static constexpr int kLCDUnit = 3;
  static constexpr int kDataPortWidth = 8;
  static constexpr int kPadding = 10;
  static constexpr int kRoundRadius = 3;
  static constexpr int kBackground = 0;
  static constexpr int kShow = 1;
  static constexpr QColor kBackgroundColor = QColor(255, 255, 255);
  static constexpr QColor kShowColor = QColor(0, 0, 0);
  static constexpr QColor kBorderColor = QColor(67, 80, 108);

public:
  GraphicLCDRawComponent(QWidget *parent = nullptr);
  virtual ~GraphicLCDRawComponent();

  void reset() override;
  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void initPorts() override;
  // void setPortsIndex() override;

  void processData();
  void processCommand();

  /// @brief show the data on the screen
  void writeToImage();

private:
  uint8_t db_;
  bool di_;
  bool rw_;
  bool en_;
  bool cs1_;
  bool cs2_;
  bool rst_;

  int x_addr_;
  int y_addr_;
  ushort mem_addr_;
  QBitArray mem_;
  // QBitArray screen_;
  QImage *image_;
};

} // namespace rabbit_App::component

#endif // GRAPHIC_LCD_COMPONENT_H