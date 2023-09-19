#include <cassert>

#include <QDrag>
#include <QDropEvent>
#include <QEvent>
#include <QMimeData>
#include <QUuid>

#include "Components/AbstractComponent.h"
#include "Components/ComponentsPanel.h"

using namespace rabbit_App::component;

ComponentsPanel::ComponentsPanel(QWidget *parent) : QWidget(parent) {
  setAcceptDrops(true);
  initConnections();
}

ComponentsPanel::~ComponentsPanel() {}

void ComponentsPanel::resetAllComponents() {
  for (auto component : components_) {
    component->reset();
  }
}

void ComponentsPanel::removeAllComponents() {
  for (auto component : components_) {
    removeComponent(component);
  }
}

void ComponentsPanel::setConstraintFilePath(
    const QString &constraint_file_path) {
  for (auto component : components_) {
    component->setConstraintFilePath(constraint_file_path);
  }
}

void ComponentsPanel::onUpdateGui() {
  for (auto component : components_) {
    component->update();
  }
}
void ComponentsPanel::onFrequencyChanged(int freq) {
  for (auto component : components_) {
    component->rawComponent()->setFrequency(freq);
  }
}

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

    QString uuid;
    QPoint drag_start_pos;
    dataStream >> uuid >> drag_start_pos;

    auto component = components_.value(QUuid(uuid));
    assert(component != nullptr);

    // Get the nearest grid position and set the component's position to it.
    auto origin_corner = component->pos();
    auto mouse_pos = event->position().toPoint();
    auto x_distance = mouse_pos.x() - origin_corner.x();
    auto y_distance = mouse_pos.y() - origin_corner.y();
    auto current_corner = origin_corner + QPoint(x_distance, y_distance);
    auto grid_row = current_corner.y() / grid_height_;
    auto grid_col = current_corner.x() / grid_width_;
    component->setGeometry(grid_width_ * grid_col, grid_height_ * grid_row,
                           component->width(), component->height());

    event->setDropAction(Qt::MoveAction);
    event->accept();
    emit componentsModified();
  } else {
    event->ignore();
  }
}

void ComponentsPanel::initConnections() {
  connect(this, &ComponentsPanel::gridSizeChanged, this,
          &ComponentsPanel::onGridSizeChanged);
}

void ComponentsPanel::onGridSizeChanged(int grid_width, int grid_height) {
  for (const auto &component : components_) {
    auto component_grid_size = component->gridOccupation();
    auto component_width = component_grid_size.width() * grid_width;
    auto component_height = component_grid_size.height() * grid_height;
    component->setFixedSize(component_width, component_height);

    auto component_grid_pos = componentGridCoords(component);
    component->setGeometry(grid_width * component_grid_pos.x(),
                           grid_height * component_grid_pos.y(),
                           component_width, component_height);
  }
  grid_width_ = grid_width;
  grid_height_ = grid_height;
}

void ComponentsPanel::appendComponent(AbstractComponent *component,
                                      int gird_row, int grid_col) {
  component->setParent(this);
  components_.insert(component->uuid(), component);

  auto component_grid_size = component->gridOccupation();
  auto component_width = component_grid_size.width() * grid_width_;
  auto component_height = component_grid_size.height() * grid_height_;
  component->setFixedSize(component_width, component_height);

  component->setGeometry(grid_width_ * grid_col, grid_height_ * gird_row,
                         component_width, component_height);

  component->show();
  connect(component, &AbstractComponent::componentRemoved, this,
          &ComponentsPanel::removeComponent);
  connect(component, &AbstractComponent::getConstraintFilePath, this,
          [this]() { emit getConstraintFilePath(); });
  connect(component, &AbstractComponent::settingsChangedSignal, this,
          [this]() { emit componentsModified(); });
}

void ComponentsPanel::appendComponent(AbstractComponent *component) {
  emit componentsModified();
  return appendComponent(component, 0, 0);
}

void ComponentsPanel::removeComponent(AbstractComponent *component) {
  components_.erase(components_.find(component->uuid()));
  component->deleteLater();
  emit componentsModified();
}

uint64_t ComponentsPanel::getWriteData() const {
  uint64_t write_data = 0;
  for (auto component : components_) {
    write_data |= component->getWriteData();
  }
  return write_data;
}
