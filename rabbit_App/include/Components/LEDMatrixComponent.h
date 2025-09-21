#pragma once
#ifndef LED_MATRIX_COMPONENT_H
#define LED_MATRIX_COMPONENT_H

#include <QLineEdit>
#include <QObject>
#include <QPair>
#include <QTime>
#include <QTimer>
#include <QVector>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include "Components/ComponentSettingsDialog.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(LED4x4Matrix)
COMPONENT_CLASS_DECLARATION(LED8x8Matrix)
COMPONENT_CLASS_DECLARATION(LED16x16Matrix)

/// @brief SingleLED class
/// This class implements the single LED in  Matrix.
class SingleLED final : public QWidget {
  Q_OBJECT

public:
  // constance
  static constexpr float kDefaultRadius = 12.0;
  static constexpr float kDefaultSize = 26.0;
  static constexpr QColor kOffColor = QColor(61, 97, 152);
  static constexpr QColor kOnColor = QColor(255, 255, 255);

public:
  SingleLED(QWidget *parent = nullptr);
  SingleLED(float radius, QWidget *parent = nullptr);
  virtual ~SingleLED();

  void setLEDState(bool state);
  void setLEDLevel(float level);
  void setVisionPersistence(uint vision_persistence);
  void reset();
  // void onTimerTimeout();

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  uint vision_persistence_ = 100;

  /// @brief The counter of vision persistence time.
  uint counter_ = 0;

  /// @brief Cached frame count used to approximate the persistence duration.
  uint persistence_frames_ = 0;

  /// @brief Whether the LED is on vision persistence.
  bool on_persistence_ = false;

  bool state_ = false;

  /// @brief The level of the LED, unused in version 1.0.1
  float level_;

  /// @brief Remember the last lit intensity for fade-out calculations.
  float persisted_level_ = 0.0f;

  float radius_;
  float size_;
}; // class SingleLED

/// @brief LED4x4MatrixRawComponent class
/// This class implements the 4x4 LED Matrix component.
class LED4x4MatrixRawComponent : public AbstractRawComponent {
  Q_OBJECT

  static constexpr QColor kBackGroundColor = QColor(67, 80, 108);

public:
  LED4x4MatrixRawComponent(QWidget *parent = nullptr, bool is_4x4 = true);
  virtual ~LED4x4MatrixRawComponent();

  void reset() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

  void updateAllLEDs();

  uint visionPersistence() const noexcept { return vision_persistence_; }
  void setVisionPersistence(int vision_persistence) override;
  // void setPortsIndex() override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void initPorts() override;
  void initLayout(float single_led_radius = SingleLED::kDefaultRadius);

  /// @brief Set the number of columns and rows of the matrix.
  /// If you want to add a LEDMatrix with different size,
  /// use this function in the derived class constuctor.
  void setColumnNum(int column_num) { column_num_ = column_num; }

  /// @brief Set the number of columns and rows of the matrix.
  void setRowNum(int row_num) { row_num_ = row_num; }

  // uint vision_persistence_ = 100;

private:
  /// @brief Vector of the LEDs.
  QVector<SingleLED *> leds_vec_;
  int column_num_;
  int row_num_;

}; // class LED4x4MatrixRawComponent

/// @brief LED8x8MatrixRawComponent class
/// This class implements the 8x8 LED Matrix component.
/// Inherited from LED4x4MatrixRawComponent class.
class LED8x8MatrixRawComponent : public LED4x4MatrixRawComponent {
  Q_OBJECT

public:
  LED8x8MatrixRawComponent(QWidget *parent = nullptr);
  ~LED8x8MatrixRawComponent();

}; // class LED8x8MatrixRawComponent

/// @brief LED16x16MatrixRawComponent class
/// This class implements the 16x16 LED Matrix component.
/// Inherited from LED4x4MatrixRawComponent class.
class LED16x16MatrixRawComponent : public LED4x4MatrixRawComponent {
  Q_OBJECT

public:
  LED16x16MatrixRawComponent(QWidget *parent = nullptr);
  ~LED16x16MatrixRawComponent();

}; // class LED16x16MatrixRawComponent

/// @brief LEDMatrixSettingsDialog class
/// This class is used to display the LED Matrix settings dialog.
using LEDMatrixSettingsDialog =
    ComponentSettingsDialogWithFeatures<SettingsFeature::ActiveMode,
                                        SettingsFeature::VisionPersistence>;

} // namespace rabbit_App::component

#endif // LED_MATRIX_COMPONENT_H
