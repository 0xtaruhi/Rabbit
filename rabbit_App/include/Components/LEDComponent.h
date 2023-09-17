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
  void onSetLevel(float level) { level_ = level; }

private:
  QLabel *led_picture_;
  // bool is_on_ = false;
  float level_ = 0.0f;
};

class LEDSettingsDialog : public ActiveModeSettingsDialog,
                          public ColorSettingsDialog {
  Q_OBJECT

public:
  LEDSettingsDialog(AbstractComponent *component, QWidget *parent = nullptr);
  virtual ~LEDSettingsDialog();

protected:
  void acceptDerivedClassSettings() override;

private:
  // ACTIVE_SETTING_DECLARATION_PRIVATE_MEMBER
  // COLOR_SETTING_DECLARATION_PRIVATE_MEMBER

}; // class LEDSettingsDialog

} // namespace rabbit_App::component

#endif // LED_COMPONENT_H
