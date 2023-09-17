#include "Components/ComponentsFactory.h"
#include "Components/Components.h"

using namespace rabbit_App::component;

AbstractComponent *ComponentsFactory::create(const QString &component_name,
                                             QWidget *parent) {
  AbstractComponent *new_component = nullptr;
  if (component_name == "LED") {
    new_component = new LEDComponent(parent);
  } else if (component_name == "TextLCD") {
    new_component = new TextLCDComponent(parent);
  } else if (component_name == "Switch") {
    new_component = new SwitchComponent(parent);
  } else if (component_name == "Button") {
    new_component = new ButtonComponent(parent);
  } else if (component_name == "SegmentDisplay") {
    new_component = new SegmentDisplayComponent(parent);
  } else if (component_name == "FourDigitSegmentDisplay") {
    new_component = new FourDigitSegmentDisplayComponent(parent);
  } else if (component_name == "LED4x4Matrix") {
    new_component = new LED4x4MatrixComponent(parent);
  } else if (component_name == "LED8x8Matrix") {
    new_component = new LED8x8MatrixComponent(parent);
  } else if (component_name == "LED16x16Matrix") {
    new_component = new LED16x16MatrixComponent(parent);
  } else if (component_name == "KeyPad") {
    new_component = new KeyPadComponent(parent);
  } else if (component_name == "SmallKeyPad") {
    new_component = new SmallKeyPadComponent(parent);
  } else if (component_name == "GraphicLCD") {
    new_component = new GraphicLCDComponent(parent);
  } else if (component_name == "RotaryButton") {
    new_component = new RotaryButtonComponent(parent);
    // Add new component here :
    // } else if (component_name == "New Component Name") {
    //   new_component = new NewComponentClass(parent);
  } else {
    std::runtime_error("Unknown component type");
  }
  return new_component;
}
