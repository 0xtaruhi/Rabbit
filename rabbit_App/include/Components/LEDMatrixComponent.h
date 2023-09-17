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
#include "ThreadTimer.h"


namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(LED4x4Matrix)
COMPONENT_CLASS_DECLARATION(LED8x8Matrix)
COMPONENT_CLASS_DECLARATION(LED16x16Matrix)

class SingleLED final : public QWidget {
  Q_OBJECT

public:
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
  void setFrequency(int frequency) noexcept { frequency_ = frequency; }
  void reset();
  // void onTimerTimeout();

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  uint vision_persistence_ = 100;
  int frequency_ = 1;
  uint counter_ = 0;
  bool on_persistence_ = false;
  bool state_ = false;
  float level_;

  float radius_;
  float size_;
}; // class SingleLED

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
  void setFrequency(int frequency) noexcept override;
  // void setPortsIndex() override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void initPorts() override;
  void initLayout(float single_led_radius = SingleLED::kDefaultRadius);

  void setColumnNum(int column_num) { column_num_ = column_num; }
  void setRowNum(int row_num) { row_num_ = row_num; }

  // uint vision_persistence_ = 100;

private:
  // QMap<QPair<int, int>, SingleLED *> leds_map_;
  QVector<SingleLED *> leds_vec_;
  int column_num_;
  int row_num_;

}; // class LED4x4MatrixRawComponent

class LED8x8MatrixRawComponent : public LED4x4MatrixRawComponent {
  Q_OBJECT

public:
  LED8x8MatrixRawComponent(QWidget *parent = nullptr);
  ~LED8x8MatrixRawComponent();

}; // class LED8x8MatrixRawComponent

class LED16x16MatrixRawComponent : public LED4x4MatrixRawComponent {
  Q_OBJECT

public:
  LED16x16MatrixRawComponent(QWidget *parent = nullptr);
  ~LED16x16MatrixRawComponent();

}; // class LED16x16MatrixRawComponent

// class LEDMatrixSettingsDialog : public ComponentSettingsDialog {
//   Q_OBJECT

// public:
//   LEDMatrixSettingsDialog(AbstractComponent *component,
//                           QWidget *parent = nullptr);
//   virtual ~LEDMatrixSettingsDialog();

// protected:
//   void acceptDerivedClassSettings() override;

// private:
//   QLineEdit *vision_persistence_edit_;
//   ACTIVE_SETTING_DECLARATION_PRIVATE_MEMBER

// }; // class LEDMatrixSettingsDialog

class LEDMatrixSettingsDialog : public ActiveModeSettingsDialog,
                                   public VisionPersistenceSettingsDialog {
  Q_OBJECT

public:
  LEDMatrixSettingsDialog(AbstractComponent *component,
                             QWidget *parent = nullptr);
  virtual ~LEDMatrixSettingsDialog();

protected:
  void acceptDerivedClassSettings() override;

}; // class LEDMatrixSettingsDialog

} // namespace rabbit_App::component

#endif // LED_MATRIX_COMPONENT_H