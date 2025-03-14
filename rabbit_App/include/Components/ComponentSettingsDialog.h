#pragma once
#include "Components/ComponentSettingsDialog.h"
#include "qlineedit.h"
#include "qradiobutton.h"
#include "qwidget.h"
#include <utility>
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
#include "Ports/Port.h"
#include "Ports/PortsFileReader.h"

namespace rabbit_App::component {

class AbstractComponent;

/// @brief ComponentSettingsDialog class
/// It is used to display the component settings dialog.
/// It can be used to display the basic settings and ports settings.
/// If you want to add more settings, you can inherit from this class
class ComponentSettingsDialog : public QDialog {
  // Q_OBJECT

  constexpr static int kWindowMinWidth = 480;

public:
  /// @brief Constructor for ComponentSettingsDialog
  /// @param component Pointer to the AbstractComponent
  /// @param parent Pointer to the parent widget
  ComponentSettingsDialog(AbstractComponent *component,
                          QWidget *parent = nullptr);

  /// @brief Destructor for ComponentSettingsDialog
  ~ComponentSettingsDialog();

  /// @brief Map of all supported colors
  const static QMap<QString, QColor> all_supported_color;

protected:
  AbstractComponent *component_;
  bool is_modifieds_ = false;

  /// @brief Add a widget to the basic settings group.
  /// @param widget Pointer to the widget to be added
  void appendSettingWidget(QWidget *widget);

  /// @brief Add a layout to the basic settings group.
  /// @param layout Pointer to the layout to be added
  void appendSettingLayout(QLayout *layout);

  /// @brief Accept the dialog when the user clicks the OK button.
  void accept() override;

private:
  /// @brief Get the names of the component ports.
  /// @return List of component port names
  const QList<QString> &getComponentPortsNames();

  /// @brief Initialize the ports reader.
  void initPortsReader();

  /// @brief Initialize the user interface.
  void initUi();

  /// @brief Initialize the connections.
  void initConnections();

  /// @brief Initialize the table view.
  void initTable();

  /// @brief Add a row to the table view.
  /// @param ports Vector of ports
  /// @param port_type Type of the port
  void addRow(const QVector<ports::Port> &ports, ports::PortType port_type);

  /// @brief Create a combo box with the given ports vector.
  /// @param vec Vector of ports
  /// @return Pointer to the created combo box
  QComboBox *creatCombobox(const QVector<ports::Port> &vec);

  /// @brief Create a model item with the given text.
  /// @param text Text for the model item
  /// @return Pointer to the created model item
  QStandardItem *createItem(const QString &text);

  /// @brief Find the HDL port name of the given pin name.
  /// @param vec Vector of ports
  /// @param pin_name Name of the pin
  /// @return HDL port name
  const QString findHdlPortName(const QVector<ports::Port> &vec,
                                const QString &pin_name);

private:
  QGroupBox *basic_settings_group_; ///< Basic settings group which contains the basic settings like name, color, etc.
  QVBoxLayout *basic_settings_layout_; ///< Layout of the basic settings group
  QLineEdit *component_name_edit_; ///< Name line edit

  QTableView *table_view_; ///< Table view to display the ports
  QStandardItemModel *model_; ///< Model of the table view

  QPushButton *ok_button_; ///< OK button
  QPushButton *cancel_button_; ///< Cancel button

  QList<QString> original_ports_names_; ///< Original ports names, used to check if the ports names are modified

  rabbit_App::ports::PortsFileReader *ports_file_reader_; ///< Ports file reader, used to read the ports file to get the ports names
}; // class ComponentSettingsDialog

/// @brief Enum class for settings features
enum class SettingsFeature {
  ActiveMode,
  VisionPersistence,
  Color,
};

/// @brief Template class for settings feature widget
/// @tparam Derived Derived class type
template <typename Derived> class SettingsFeatureWidget : public QWidget {
public:
  using QWidget::QWidget;

  /// @brief Accept the settings feature
  /// @param component Pointer to the AbstractComponent
  void accept(AbstractComponent *component) {
    static_cast<Derived *>(this)->accept(component);
  }
}; // class SettingsFeatureWidget

/// @brief Template struct for widget of feature helper
/// @tparam F Settings feature type
template <SettingsFeature F> struct WidgetOfFeatureHelper {
  using type = void;
};

/// @brief Template alias for widget of feature
/// @tparam F Settings feature type
template <SettingsFeature F>
using WidgetOfFeature = typename WidgetOfFeatureHelper<F>::type;

/// @brief Class for active mode settings feature widget
class ActiveModeSettingsFeatureWidget
    : public SettingsFeatureWidget<ActiveModeSettingsFeatureWidget> {
  // Q_OBJECT
public:
  /// @brief Constructor for ActiveModeSettingsFeatureWidget
  /// @param component Pointer to the AbstractComponent
  /// @param parent Pointer to the parent widget
  ActiveModeSettingsFeatureWidget(AbstractComponent *component,
                                  QWidget *parent = nullptr);

  /// @brief Accept the active mode settings feature
  /// @param component Pointer to the AbstractComponent
  void accept(AbstractComponent *component);

private:
  QRadioButton *active_high_radio_button_; ///< Active high radio button
};
template <> struct WidgetOfFeatureHelper<SettingsFeature::ActiveMode> {
  using type = ActiveModeSettingsFeatureWidget;
};

/// @brief Class for vision persistence settings feature widget
class VisionPersistenceSettingsFeatureWidget
    : public SettingsFeatureWidget<VisionPersistenceSettingsFeatureWidget> {
public:
  /// @brief Constructor for VisionPersistenceSettingsFeatureWidget
  /// @param component Pointer to the AbstractComponent
  /// @param parent Pointer to the parent widget
  VisionPersistenceSettingsFeatureWidget(AbstractComponent *component,
                                         QWidget *parent = nullptr);

  /// @brief Accept the vision persistence settings feature
  /// @param component Pointer to the AbstractComponent
  void accept(AbstractComponent *component);

private:
  QLineEdit *vision_persistence_edit_; ///< Vision persistence line edit
};
template <> struct WidgetOfFeatureHelper<SettingsFeature::VisionPersistence> {
  using type = VisionPersistenceSettingsFeatureWidget;
};

/// @brief Class for color settings feature widget
class ColorSettingsFeatureWidget
    : public SettingsFeatureWidget<ColorSettingsFeatureWidget> {
public:
  /// @brief Constructor for ColorSettingsFeatureWidget
  /// @param component Pointer to the AbstractComponent
  /// @param parent Pointer to the parent widget
  ColorSettingsFeatureWidget(AbstractComponent *component,
                             QWidget *parent = nullptr);

  /// @brief Accept the color settings feature
  /// @param component Pointer to the AbstractComponent
  void accept(AbstractComponent *component);

private:
  QMap<QString, QComboBox *> color_map_; ///< Map of color combo boxes
};
template <> struct WidgetOfFeatureHelper<SettingsFeature::Color> {
  using type = ColorSettingsFeatureWidget;
};

/// @brief Template class for component settings dialog with features
/// @tparam Features Settings features
template <SettingsFeature... Features>
class ComponentSettingsDialogWithFeatures : public ComponentSettingsDialog {
  static constexpr auto kFeaturesNum = sizeof...(Features);

public:
  /// @brief Constructor for ComponentSettingsDialogWithFeatures
  /// @param component Pointer to the AbstractComponent
  /// @param parent Pointer to the parent widget
  ComponentSettingsDialogWithFeatures(AbstractComponent *component,
                                      QWidget *parent = nullptr)
      : ComponentSettingsDialog(component, parent) {
    initFeatureWidgets(std::make_index_sequence<kFeaturesNum>{});
  }

  /// @brief Accept the dialog with features
  void accept() override {
    (void)std::initializer_list<int>{
        (static_cast<WidgetOfFeature<Features> *>(
             features_[getFeatureIndex<Features>()])
             ->accept(component_),
         0)...};
    ComponentSettingsDialog::accept();
  }

  /// @brief Destructor for ComponentSettingsDialogWithFeatures
  virtual ~ComponentSettingsDialogWithFeatures() {}

private:
  std::array<QWidget *, kFeaturesNum> features_ = {nullptr};

  /// @brief Initialize the feature widgets
  /// @tparam Indices Indices of the features
  /// @param std::index_sequence<Indices...> Index sequence
  template <size_t... Indices>
  void initFeatureWidgets(std::index_sequence<Indices...>) {
    (void)std::initializer_list<int>{
        (features_[Indices] = new WidgetOfFeature<Features>(component_, this),
         0)...};
    for (auto feature : features_) {
      appendSettingWidget(feature);
    }
  }

  /// @brief Get the index of the feature
  /// @tparam Feature Settings feature type
  /// @return Index of the feature
  template <SettingsFeature Feature> static constexpr size_t getFeatureIndex() {
    constexpr SettingsFeature features[] = {Features...};
    for (size_t i = 0; i < sizeof...(Features); ++i) {
      if (features[i] == Feature) {
        return i;
      }
    }
    return 0;
  }
};

} // namespace rabbit_App::component

#endif // COMPONENT_SETTINGS_DIALOG_H
