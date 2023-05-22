#include "Components/ComponentAction.h"
#include "Components/Components.h"

using namespace rabbit_App::component;

ComponentAction *ComponentActionFactory::create(const QString &text,
                                                QObject *parent) {
  auto action = new ComponentAction(parent);

  if (text == "LED") {
    action->setText(text);
    action->setIcon(QIcon(":/icons/icons/components/icons8-light-94.png"));
    action->component_factory_ = []() { return new LEDComponent(); };
  } else {
    std::runtime_error("Unknown component type");
  }

  return action;
}
