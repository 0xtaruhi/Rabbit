#pragma once
#ifndef COMPONENTS_PANEL_H
#define COMPONENTS_PANEL_H

#include <QWidget>
#include "Components/AbstractComponent.h"
#include <QEvent>

namespace rabbit_App::component {
class AbstractComponent;

class ComponentsPanel : public QWidget {
  Q_OBJECT

public:
  ComponentsPanel(QWidget *parent = nullptr);
  ~ComponentsPanel();

  void appendComponent(AbstractComponent* component, int grid_row, int grid_col);

protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;

protected slots:
  void removeComponent(AbstractComponent* component);

private:
  int grid_width_ = 100;
  int grid_height_ = 100;
};

} // namespace rabbit_App::component

#endif // COMPONETNS_PANEL_H
