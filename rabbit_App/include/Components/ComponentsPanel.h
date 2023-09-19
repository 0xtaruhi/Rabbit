#pragma once
#ifndef COMPONENTS_PANEL_H
#define COMPONENTS_PANEL_H

#include <QEvent>
#include <QMap>
#include <QWidget>

#include "Components/AbstractComponent.h"

namespace rabbit_App::component {
class AbstractComponent;
class ValueUpdateController;

class ComponentsPanel : public QWidget {
  Q_OBJECT

  friend class ValueUpdateController;

public:
  ComponentsPanel(QWidget *parent = nullptr);
  virtual ~ComponentsPanel();

  uint64_t getWriteData() const;

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

  /// @brief Map a position to grid coordinates.
  /// @param pos The position to map.
  /// @return The grid coordinates.
  auto posToGrid(const QPoint &pos) const noexcept {
    return QPoint(pos.x() / grid_width_, pos.y() / grid_height_);
  }

  /// @brief Map a component's position to grid coordinates.
  /// @param component The component to map.
  /// @return The grid coordinates.
  auto componentGridCoords(AbstractComponent *component) const noexcept {
    return posToGrid(component->pos());
  }

  const auto &components() const noexcept { return components_; }

  void setGridSize(int grid_width, int grid_height) noexcept {
    emit gridSizeChanged(grid_width, grid_height);
  }

  /// @brief Reset all components. This method will call the reset() method of
  /// all components.
  void resetAllComponents();

  void removeAllComponents();

  void setConstraintFilePath(const QString &constraint_file_path);

  auto gridWidth() const noexcept { return grid_width_; }
  auto gridHeight() const noexcept { return grid_height_; }

public slots:
  void onUpdateGui();
  void onFrequencyChanged(int freq);

protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;

  void initConnections();

protected slots:
  void removeComponent(AbstractComponent *component);

private slots:
  void onGridSizeChanged(int grid_width, int grid_height);

signals:
  void updateComponent(const QVector<bool> &data);
  void gridSizeChanged(int grid_width, int grid_height);
  void componentsModified();
  void getConstraintFilePath();

protected:
  int grid_width_ = 50;
  int grid_height_ = 50;

  QHash<QUuid, AbstractComponent *> components_;
};

} // namespace rabbit_App::component

#endif // COMPONETNS_PANEL_H
