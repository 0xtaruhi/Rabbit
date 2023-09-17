
#pragma once
#ifndef COMPONENT_SETTINGS_DIALOG_H
#define COMPONENT_SETTINGS_DIALOG_H

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QRadioButton>
#include <QStandardItemModel>
#include <QTableView>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include "Ports/Port.h"
#include "Ports/PortsFileReader.h"

namespace rabbit_App::component {

class AbstractComponent;

class ComponentSettingsDialog : public QDialog {
  Q_OBJECT

  constexpr static int kWindowMinWidth = 480;

public:
  ComponentSettingsDialog(AbstractComponent *component,
                          QWidget *parent = nullptr);
  ~ComponentSettingsDialog();

  const static QMap<QString, QColor> all_supported_color;

protected:
  AbstractComponent *component_;
  bool is_modifieds_ = false;
  void appendSettingWidget(QWidget *widget);
  void appendSettingLayout(QLayout *layout);
  virtual void acceptDerivedClassSettings() {}

private:
  const QList<QString> &getComponentPortsNames();

  void initPortsReader();
  void initUi();
  void initConnections();

  void initTable();

  void addRow(const QVector<ports::Port> &ports, ports::PortType port_type);

  QComboBox *creatCombobox(const QVector<ports::Port> &map);
  QStandardItem *createItem(const QString &text);

  const QString findHdlPortName(const QVector<ports::Port> &map,
                                const QString &pin_name);

  void accept() override;

private:
  QGroupBox *basic_settings_group_;
  QVBoxLayout *basic_settings_layout_;
  QLineEdit *component_name_edit_;

  QTableView *table_view_;
  QStandardItemModel *model_;

  QPushButton *ok_button_;
  QPushButton *cancel_button_;

  QList<QString> original_ports_names_;

  rabbit_App::ports::PortsFileReader *ports_file_reader_;

}; // class ComponentSettingsDialog

class ActiveModeSettingsDialog : virtual public ComponentSettingsDialog {
  Q_OBJECT

public:
  ActiveModeSettingsDialog(AbstractComponent *component,
                           QWidget *parent = nullptr);
  virtual ~ActiveModeSettingsDialog();

protected:
  void acceptDerivedClassSettings() override;

private:
  QRadioButton *active_high_radio_button_;

}; // class ActiveModeSettingsDialog

class VisionPersistenceSettingsDialog : virtual public ComponentSettingsDialog {
  Q_OBJECT

public:
  VisionPersistenceSettingsDialog(AbstractComponent *component,
                                  QWidget *parent = nullptr);
  virtual ~VisionPersistenceSettingsDialog();

protected:
  void acceptDerivedClassSettings() override;

private:
  QLineEdit *vision_persistence_edit_;

}; // class VisionPersistenceSettingsDialog

class ColorSettingsDialog : virtual public ComponentSettingsDialog {
  Q_OBJECT

public:
  ColorSettingsDialog(AbstractComponent *component, QWidget *parent = nullptr);
  virtual ~ColorSettingsDialog();

protected:
  void acceptDerivedClassSettings() override;

private:
  QMap<QString, QComboBox *> color_map_;

}; // class ColorSettingsDialog

} // namespace rabbit_App::component

#endif // COMPONENT_SETTINGS_DIALOG_H