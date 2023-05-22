#pragma once
#ifndef ABSTRACT_COMPONENT_H
#define ABSTRACT_COMPONENT_H

#include <QUuid>
#include <QWidget>
#include "Components/ComponentTitleBar.h"
#include <QSize>
#include <QPoint>

namespace rabbit_App::component {

/// @brief Abstract class for all components. Do not instantiate.
class AbstractComponent : public QWidget {
  Q_OBJECT

public:
  AbstractComponent(QWidget *parent = nullptr);
  ~AbstractComponent();

  constexpr const QUuid &uuid() const noexcept { return uuid_; }

  /// @brief Returns the size of the grid occupied by the component.
  /// @return The size of the grid occupied by the component.
  virtual QSize gridOccupation() const noexcept = 0;

  /// @brief Returns the raw component. The raw component is the true component
  /// that accepts inputs and displays outputs. The classes that inherit from
  /// AbstractComponent are only wrappers for the raw component.
  /// @return The raw component.
  auto rawComponent() const noexcept { return raw_component_; }

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

signals:
  void componentRemoved(AbstractComponent* component);

private:
  QUuid uuid_;
  QPoint drag_start_position_;
  ComponentTitleBar* title_bar_;

  void initMembers();
  void initLayout();
  void initConnections();

  void askForRemove();

protected:
  QWidget *raw_component_;
};

class AbstractInputComponent : public AbstractComponent {
  Q_OBJECT
public:
  AbstractInputComponent(QWidget *parent = nullptr)
      : AbstractComponent(parent) {}
};

class AbstractOutputComponent : public AbstractComponent {
  Q_OBJECT
public:
  AbstractOutputComponent(QWidget *parent = nullptr)
      : AbstractComponent(parent) {}
};

} // namespace rabbit_App::component

#endif // ABSRACT_COMPONENT_H
