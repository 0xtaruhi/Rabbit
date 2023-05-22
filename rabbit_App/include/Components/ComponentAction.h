#ifndef COMPONENT_ACTION_H
#define COMPONENT_ACTION_H

#include <QAction>

#include "Components/AbstractComponent.h"

namespace rabbit_App::component {

class ComponentActionFactory;

/// @brief A QAction that stores a factory function to create a component.
/// This class is used in the TabToolbar.
class ComponentAction : public QAction {
  Q_OBJECT

public:
  ComponentAction(QObject *parent = nullptr) : QAction(parent) {}

  /// @brief This function will return a new component.
  /// @note You should never instantiate a component directly, but
  /// always use the ComponentActionFactory::create() function to create a
  /// ComponentAction object.
  AbstractComponent *createComponent() const noexcept {
    return component_factory_();
  }

private:
  std::function<AbstractComponent *()> component_factory_;

  friend class ComponentActionFactory;
};

class ComponentActionFactory {
public:
  /// @brief Creates a new ComponentAction object with the given name.
  static ComponentAction *create(const QString &text,
                                 QObject *parent = nullptr);
};

} // namespace rabbit_App::component

#endif // COMPONENT_ACTION_H
