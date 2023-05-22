#pragma once
#ifndef COMPONENTS_PANEL_H
#define COMPONENTS_PANEL_H

#include <QEvent>
#include <QMap>
#include <QWidget>

#include "Components/AbstractComponent.h"

namespace rabbit_App::component {
class AbstractComponent;

class ComponentsPanel : public QWidget {
  Q_OBJECT

public:
  ComponentsPanel(QWidget *parent = nullptr);
  virtual ~ComponentsPanel();

  /// @brief Append a component to the panel. Manually set the position.
  /// @param component The component to append.
  /// @param grid_row The row of the grid where the component will be placed.
  /// @param grid_col The column of the grid where the component will be placed.
  /// @note The parent of the component will be set to the panel automatically.
  /// So you don't need to set the parent of the component.
  void appendComponent(AbstractComponent *component, int grid_row,
                       int grid_col);

  /// @brief Append a component to the panel. Automatically set the position.
  /// @param component The component to append.
  /// @note The parent of the component will be set to the panel automatically.
  /// So you don't need to set the parent of the component.
  void appendComponent(AbstractComponent *component);

protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;

protected slots:
  void removeComponent(AbstractComponent *component);

protected:
  int grid_width_ = 100;
  int grid_height_ = 100;

  QHash<QUuid, AbstractComponent *> components_;
};

} // namespace rabbit_App::component

#endif // COMPONETNS_PANEL_H
