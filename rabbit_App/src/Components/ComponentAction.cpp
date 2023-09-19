#include "Components/ComponentAction.h"
#include "Components/Components.h"
#include "Components/ComponentsFactory.h"

using namespace rabbit_App::component;

ComponentAction *ComponentActionFactory::create(const QString &text,
                                                QObject *parent) {
  auto action = new ComponentAction(parent);
  action->setText(text);

  action->component_factory_ = [=]() {
    return ComponentsFactory::create(text);
  };

  if (text == "LED") {
    action->setIcon(LEDComponent::componentIcon());
  } else if (text == "TextLCD") {
    action->setIcon(TextLCDComponent::componentIcon());
  } else if (text == "Switch") {
    action->setIcon(SwitchComponent::componentIcon());
  } else if (text == "Button") {
    action->setIcon(ButtonComponent::componentIcon());
  } else if (text == "SegmentDisplay") {
    action->setIcon(SegmentDisplayComponent::componentIcon());
  } else if (text == "FourDigitSegmentDisplay") {
    action->setIcon(FourDigitSegmentDisplayComponent::componentIcon());
  } else if (text == "LED4x4Matrix") {
    action->setIcon(LED4x4MatrixComponent::componentIcon());
  } else if (text == "LED8x8Matrix") {
    action->setIcon(LED8x8MatrixComponent::componentIcon());
  } else if (text == "LED16x16Matrix") {
    action->setIcon(LED16x16MatrixComponent::componentIcon());
  } else if (text == "KeyPad") {
    action->setIcon(KeyPadComponent::componentIcon());
  } else if (text == "SmallKeyPad") {
    action->setIcon(SmallKeyPadComponent::componentIcon());
  } else if (text == "GraphicLCD") {
    action->setIcon(GraphicLCDComponent::componentIcon());
  } else if (text == "RotaryButton") {
    action->setIcon(RotaryButtonComponent::componentIcon());
    // add new component icon on toolbar here :
    // } else if (text == "New Component Name") {
    //   action->setIcon(NewComponentClass::componentIcon());
  } else {
    std::runtime_error("Unknown component type");
  }

  return action;
}
