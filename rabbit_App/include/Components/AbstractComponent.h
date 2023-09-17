#pragma once
#ifndef ABSTRACT_COMPONENT_H
#define ABSTRACT_COMPONENT_H

#include <QBitArray>
#include <QPoint>
#include <QQueue>
#include <QSize>
#include <QUuid>
#include <QWidget>

#include "Components/ComponentTitleBar.h"
#include "Ports/Port.h"

namespace rabbit_App::component {

class ComponentSettingsDialog;

/// @brief Abstract class for all raw components.
/// The raw component is the true component that accepts inputs and displays
/// outputs.
class AbstractRawComponent : public QWidget {
  Q_OBJECT

public:
  AbstractRawComponent(QWidget *parent = nullptr);
  virtual ~AbstractRawComponent();

  virtual void reset() = 0;

  const auto &inputPorts() const noexcept { return input_ports_; }
  const auto &outputPorts() const noexcept { return output_ports_; }
  auto &inputPorts() noexcept { return input_ports_; }
  auto &outputPorts() noexcept { return output_ports_; }
  ports::Port &findPort(const QString &port_name, const ports::PortType type);
  void appendPort(QVector<ports::Port> &map, const QString &name,
                  const ports::PortType type);

  virtual void processReadData(QQueue<uint64_t> &read_queue) = 0;
  virtual uint64_t getWriteData() const = 0;

  bool isLowActive() const noexcept { return is_low_active_; }
  void setLowActive(bool is_low_active) noexcept {
    is_low_active_ = is_low_active;
  }
  int visionPersistence() const noexcept { return vision_persistence_; }
  virtual void setVisionPersistence(int vision_persistence) {
    vision_persistence_ = vision_persistence;
  }
  const QMap<QString, QColor> &componentColors() const noexcept {
    return component_colors_;
  }
  void setComponentColor(const QString &name, const QColor &color) noexcept {
    component_colors_[name] = color;
  }
  void
  setComponentColors(const QMap<QString, QColor> &component_colors) noexcept {
    component_colors_ = component_colors;
  }
  void appendColor(const QString &name, const QColor &color) noexcept {
    component_colors_[name] = color;
  }
  const QMap<QString, QColor> &supportedColors() const noexcept {
    return supported_colors_;
  }
  void
  setSupportedColors(const QMap<QString, QColor> &supported_colors) noexcept {
    supported_colors_ = supported_colors;
  }
  virtual void setFrequency(int frequency) noexcept { frequency_ = frequency; }

protected:
  void paintEvent(QPaintEvent *event) override = 0;
  virtual void initPorts() = 0;

  bool is_low_active_ = false;
  int vision_persistence_ = 100;

  /// @brief all uesd colors for components
  QMap<QString, QColor> component_colors_;
  /// @brief colors shown in settings dialog
  /// if component is supported to change color, settings dialog will show
  /// according to this map
  QMap<QString, QColor> supported_colors_;

  int frequency_ = 1;

protected:
  // QMap<QString, ports::Port> input_ports_;
  // QMap<QString, ports::Port> output_ports_;
  QVector<ports::Port> input_ports_;
  QVector<ports::Port> output_ports_;
  // QVector<int> ports_index_;
};

/// @brief Abstract class for all components. Do not instantiate.
class AbstractComponent : public QWidget {
  Q_OBJECT

  constexpr static auto kDefauleName = "Untitled";

public:
  AbstractComponent(QWidget *parent = nullptr);
  virtual ~AbstractComponent();

  constexpr const QUuid &uuid() const noexcept { return uuid_; }

  /// @brief Returns the size of the grid occupied by the component.
  /// @return The size of the grid occupied by the component.
  virtual QSize gridOccupation() const noexcept = 0;

  /// @brief Returns the raw component. The raw component is the true component
  /// that accepts inputs and displays outputs. The classes that inherit from
  /// AbstractComponent are only wrappers for the raw component.
  /// @return The raw component.
  auto rawComponent() noexcept { return raw_component_; }
  auto componentName() const noexcept { return component_name_; }
  void setComponentName(const QString &name) noexcept {
    component_name_ = name;
    title_bar_->setTitle(name);
  }
  const QString constraintFilePath() const noexcept {
    return constraint_file_path_;
  }
  void setConstraintFilePath(const QString &path) noexcept {
    constraint_file_path_ = path;
  }

  virtual QString componentType() const noexcept = 0;

  const auto &inputPorts() const noexcept {
    return raw_component_->inputPorts();
  }
  const auto &outputPorts() const noexcept {
    return raw_component_->outputPorts();
  }
  auto &inputPorts() noexcept { return raw_component_->inputPorts(); }
  auto &outputPorts() noexcept { return raw_component_->outputPorts(); }

  void reset() { raw_component_->reset(); }

  void askForConstraintFilePath() { emit getConstraintFilePath(); }
  void settingsChanged() { emit settingsChangedSignal(); }

  uint64_t getWriteData() const;

protected:
  void paintEvent(QPaintEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

signals:
  void componentRemoved(AbstractComponent *component);
  void getConstraintFilePath();
  void settingsChangedSignal();

public slots:
  // void onUpdateComponent(const QVector<bool>& data);
  void onUpdateGui() { update(); }

protected slots:
  virtual void onSettingsBtnClicked();

private:
  QUuid uuid_;
  QPoint drag_start_position_;
  ComponentTitleBar *title_bar_;
  QString component_name_;
  QString constraint_file_path_;

  void initMembers();
  void initLayout();
  void initConnections();

  void askForRemove();

protected:
  AbstractRawComponent *raw_component_;
};

} // namespace rabbit_App::component

#endif // ABSRACT_COMPONENT_H
