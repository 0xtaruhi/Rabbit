#pragma once
#ifndef LED_COMPONENT_H
#define LED_COMPONENT_H

#include <QLabel>
#include <QRadioButton>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include "Components/ComponentSettingsDialog.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(LED)

/// @brief LEDRawComponent class
/// This class implements the LED component.
class LEDRawComponent : public AbstractRawComponent {
  Q_OBJECT

public:
  LEDRawComponent(QWidget *parent = nullptr);
  virtual ~LEDRawComponent();

  void reset() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void initPorts() override;

public slots:
  // void onSwitchLED(bool is_on);

  /// @brief Set the level of the LED.
  /// Note that LED has level but not state,
  /// so when processing the read data, take average.
  void onSetLevel(float level) { level_ = level; }

private:
  QLabel *led_picture_;
  // bool is_on_ = false;
  float level_ = 0.0f;
};

/// @brief LEDSettingsDialog class
/// This class is used to display the LED settings dialog.
using LEDSettingsDialog =
    ComponentSettingsDialogWithFeatures<SettingsFeature::ActiveMode,
                                        SettingsFeature::Color>;

} // namespace rabbit_App::component

#endif // LED_COMPONENT_H
