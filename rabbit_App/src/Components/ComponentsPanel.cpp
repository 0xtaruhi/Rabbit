#include "Components/ComponentsPanel.h"
#include "Components/AbstractComponent.h"
#include "qevent.h"
#include <QDrag>
#include <QEvent>
#include <QMimeData>

using namespace rabbit_App::component;

ComponentsPanel::ComponentsPanel(QWidget *parent) : QWidget(parent) {
  setAcceptDrops(true);
}

ComponentsPanel::~ComponentsPanel() {}

void ComponentsPanel::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasFormat("application/x-abstractcomponent")) {
    event->acceptProposedAction();
  }
}

void ComponentsPanel::dragMoveEvent(QDragMoveEvent *event) {
  event->acceptProposedAction();
}

void ComponentsPanel::dropEvent(QDropEvent *event) {
  if (event->mimeData()->hasFormat("application/x-abstractcomponent")) {
    QByteArray itemData =
        event->mimeData()->data("application/x-abstractcomponent");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString componentName;
    QPoint offset;
    dataStream >> componentName >> offset;

    //! TODO

    event->setDropAction(Qt::MoveAction);
    event->accept();
  } else {
    event->ignore();
  }
}

void ComponentsPanel::appendComponent(AbstractComponent *component,
                                      int gird_row, int grid_col) {
  component->setParent(this);
  auto component_grid_size = component->gridOccupation();
  auto component_width = component_grid_size.width() * grid_width_;
  auto component_height = component_grid_size.height() * grid_height_;
  component->setFixedSize(component_width, component_height);

  component->setGeometry(grid_width_ * grid_col, grid_height_ * gird_row,
                         component_width, component_height);

  component->show();
  connect(component, &AbstractComponent::componentRemoved, this,
          &ComponentsPanel::removeComponent);
}

void ComponentsPanel::removeComponent(AbstractComponent *component) {
  component->deleteLater();
}