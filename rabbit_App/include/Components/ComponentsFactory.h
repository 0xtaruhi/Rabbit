#ifndef COMPONENTS_FACTORY_H
#define COMPONENTS_FACTORY_H

#include "Components/AbstractComponent.h"

namespace rabbit_App::component {

class ComponentsFactory {
public:
  static AbstractComponent *create(const QString &component_name,
                                   QWidget *parent = nullptr);
};

} // namespace rabbit_App::component

#endif // COMPONENTS_FACTORY_H
