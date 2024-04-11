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

  // Pure virtual functions

  /// @brief Reset the component. Pure virtual function. This function will be
  /// called when run/stop button are clicked.
  virtual void reset() = 0;

  /// @brief process read data. This is a pure virtual function.
  /// @param read_queue The queue of read data.
  virtual void processReadData(QQueue<uint64_t> &read_queue) = 0;

  /// @brief process write data. This is a pure virtual function.
  virtual uint64_t getWriteData() const = 0;

  // Ports

  /// @brief Get the input ports of the component.
  const auto &inputPorts() const noexcept { return input_ports_; }

  /// @brief Get the output ports of the component.
  const auto &outputPorts() const noexcept { return output_ports_; }

  /// @brief Get the input ports of the component.
  auto &inputPorts() noexcept { return input_ports_; }

  /// @brief Get the output ports of the component.
  auto &outputPorts() noexcept { return output_ports_; }

  /// @brief Get the port of the component by name.
  /// @param port_name The name of the port.
  /// @param type The type of the port.
  /// @return The port of the component.
  ports::Port &findPort(const QString &port_name, const ports::PortType type);

  /// @brief Append a port to the component.
  /// @param vec The vector of ports : input_ports_ or output_ports_.
  /// @param name The name of the port in the component.
  /// @param type The type of the port.
  void appendPort(QVector<ports::Port> &vec, const QString &name,
                  const ports::PortType type);

  // Settings
  /// @brief Whether the component is low active.
  bool isLowActive() const noexcept { return is_low_active_; }
  /// @brief Set whether the component is low active.
  /// @param is_low_active Whether the component is low active.
  void setLowActive(bool is_low_active) noexcept {
    is_low_active_ = is_low_active;
  }

  /// @brief Get the vision persistence of the component.
  int visionPersistence() const noexcept { return vision_persistence_; }
  /// @brief Set the vision persistence of the component.
  /// This function is virtual because some components may have a child which
  /// also has vision persistence.
  /// @param vision_persistence The vision persistence of the component.
  virtual void setVisionPersistence(int vision_persistence) {
    vision_persistence_ = vision_persistence;
  }

  /// @brief Get the component colors.
  QMap<QString, QColor> &componentColors() noexcept {
    return component_colors_;
  }
  /// @brief Set the component colors.
  /// @param name The name of the component.
  void setComponentColor(const QString &name, const QColor &color) noexcept {
    component_colors_[name] = color;
  }
  /// @brief Set the component colors.
  /// @param component_colors The component colors.
  void
  setComponentColors(const QMap<QString, QColor> &component_colors) noexcept {
    component_colors_ = component_colors;
  }
  /// @brief Append a color of one part of the component.
  void appendColor(const QString &name, const QColor &color) noexcept {
    component_colors_[name] = color;
  }
  /// @brief Get the supported colors of the component.
  const QMap<QString, QColor> &supportedColors() const noexcept {
    return supported_colors_;
  }
  /// @brief Set the supported colors of the component.
  /// @param supported_colors The supported colors of the component.
  void
  setSupportedColors(const QMap<QString, QColor> &supported_colors) noexcept {
    supported_colors_ = supported_colors;
  }

  // /// @brief Set the frequency of the component.
  // virtual void setFrequency(int frequency) noexcept { frequency_ = frequency;
  // }

protected:
  /// @brief Paint the component.
  /// @param event The paint event.
  void paintEvent(QPaintEvent *event) override = 0;

  /// @brief Initialize the ports of the component. This is a pure virtual
  /// function.
  virtual void initPorts() = 0;

  /// @brief Whether the component is low active.
  bool is_low_active_ = false;
  /// @brief The vision persistence of the component.
  int vision_persistence_ = 100;

  /// @brief all uesd colors for components
  QMap<QString, QColor> component_colors_;
  /// @brief colors shown in settings dialog
  /// if component is supported to change color, settings dialog will show
  /// according to this map
  QMap<QString, QColor> supported_colors_;

  // int frequency_ = 1;

protected:
  // QMap<QString, ports::Port> input_ports_;
  // QMap<QString, ports::Port> output_ports_;

  /// @brief The input ports of the component. This is a vector, so that the
  /// ports append order is important.
  QVector<ports::Port> input_ports_;
  /// @brief The output ports of the component. This is a vector, so that the
  /// ports append order is important.
  QVector<ports::Port> output_ports_;

  // QVector<int> ports_index_;
};

/// @brief Abstract class for all components. Do not instantiate.
class AbstractComponent : public QWidget {
  Q_OBJECT

public:
  /// @brief The default name of the component.
  constexpr static auto kDefauleName = "Untitled";

public:
  AbstractComponent(QWidget *parent = nullptr);
  virtual ~AbstractComponent();

  /// @brief Returns the uuid of the component.
  /// @return The uuid of the component.
  constexpr const QUuid &uuid() const noexcept { return uuid_; }

  /// @brief Returns the size of the grid occupied by the component.
  /// @return The size of the grid occupied by the component.
  virtual QSize gridOccupation() const noexcept = 0;

  /// @brief Returns the raw component. The raw component is the true component
  /// that accepts inputs and displays outputs. The classes that inherit from
  /// AbstractComponent are only wrappers for the raw component.
  /// @return The raw component.
  auto rawComponent() noexcept { return raw_component_; }

  /// @brief Get the name of the component.
  auto componentName() const noexcept { return component_name_; }
  /// @brief Set the name of the component.
  void setComponentName(const QString &name) noexcept {
    component_name_ = name;
    title_bar_->setTitle(name);
  }
  /// @brief Get the constraint file path of the component.
  const QString constraintFilePath() const noexcept {
    return constraint_file_path_;
  }
  /// @brief Set the constraint file path of the component.
  /// This function will be called when you want to connect the
  /// ports.
  void setConstraintFilePath(const QString &path) noexcept {
    constraint_file_path_ = path;
  }

  /// @brief Returns the type of the component. Pure virtual.
  /// This function is written in macro.
  virtual QString componentType() const noexcept = 0;

  /// @brief Returns the type of the component.
  const auto &inputPorts() const noexcept {
    return raw_component_->inputPorts();
  }
  /// @brief Returns the type of the component.
  const auto &outputPorts() const noexcept {
    return raw_component_->outputPorts();
  }
  /// @brief Returns the type of the component.
  auto &inputPorts() noexcept { return raw_component_->inputPorts(); }
  /// @brief Returns the type of the component.
  auto &outputPorts() noexcept { return raw_component_->outputPorts(); }

  /// @brief Reset the component.
  void reset() { raw_component_->reset(); }

  /// @brief Ask the project manager for the constraint file path.
  void askForConstraintFilePath() { emit getConstraintFilePath(); }
  /// @brief When component settings or position changed, emit this signal.
  void settingsChanged() { emit settingsChangedSignal(); }

  /// @brief Returns the write data of the component. Called by
  /// VLFDRunningHandler.
  uint64_t getWriteData() const;

protected:
  /// @brief Paint the component.
  void paintEvent(QPaintEvent *event) override;
  /// @brief Context menu event. Do nothing in this version.
  void contextMenuEvent(QContextMenuEvent *event) override;
  /// @brief Mouse press event.
  void mousePressEvent(QMouseEvent *event) override;
  /// @brief Mouse move event.
  void mouseMoveEvent(QMouseEvent *event) override;
  /// @brief Key press event
  void keyPressEvent(QKeyEvent *event) override;
  /// @brief Key release event
  void keyReleaseEvent(QKeyEvent *event) override;

signals:
  /// @brief Emit this signal to remove the component.
  void componentRemoved(AbstractComponent *component);
  /// @brief Emit this signal to get the constraint file path.
  void getConstraintFilePath();
  /// @brief Emit this signal when component settings or position changed.
  void settingsChangedSignal();

public slots:
  // void onUpdateComponent(const QVector<bool>& data);

  /// @brief Update the component.
  void onUpdateGui() { update(); }

protected slots:
  /// @brief Slot called when the setting button is clicked.
  virtual void onSettingsBtnClicked();

private:
  /// @brief uuid of the component.
  QUuid uuid_;
  /// @brief The position of the component when dragging start.
  QPoint drag_start_position_;
  /// @brief The title bar of the component.
  ComponentTitleBar *title_bar_;
  /// @brief The name of the component.
  QString component_name_;
  /// @brief The constraint file path of the component.
  QString constraint_file_path_;

  /// @brief Initialize the members of the component.
  void initMembers();
  /// @brief Initialize the layout of the component.
  void initLayout();
  /// @brief Initialize the connections of the component.
  void initConnections();

  /// @brief Ask whether to remove the component.
  /// This function will be called when the delete button is clicked.
  void askForRemove();

protected:
  /// @brief The raw component.
  AbstractRawComponent *raw_component_;
};

} // namespace rabbit_App::component

#endif // ABSRACT_COMPONENT_H
