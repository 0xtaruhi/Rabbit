#ifndef COMPONENT_ACTION_H
#define COMPONENT_ACTION_H

#include "Components/AbstractComponent.h"
#include <QAction>

namespace rabbit_App::component {

class ComponentActionFactory;

class ComponentAction : public QAction {
  Q_OBJECT

public:
  ComponentAction(QObject *parent = nullptr) : QAction(parent) {}

  AbstractComponent *createComponent() const noexcept {
    return component_factory_();
  }

private:
  std::function<AbstractComponent *()> component_factory_;

  friend class ComponentActionFactory;
};

class ComponentActionFactory {
public:
  static ComponentAction *create(const QString &text,
                                 QObject *parent = nullptr);
};

} // namespace rabbit_App::component

#endif // COMPONENT_ACTION_H
