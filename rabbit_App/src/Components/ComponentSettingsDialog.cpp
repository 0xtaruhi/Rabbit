#include <QFile>
#include <QHeaderView>
#include <QPushButton>

#include "Components/ComponentMacro.h"
#include "Components/ComponentSettingsDialog.h"
#include "Ports/PinInfo.h"
#include "Ports/Port.h"
#include "Ports/PortsFileReader.h"
#include "qboxlayout.h"
#include "qcombobox.h"
#include "qdebug.h"
#include "qdialog.h"
#include "qhash.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qmessagebox.h"
#include "qstandarditemmodel.h"

using namespace rabbit_App::component;

const QMap<QString, QColor> ComponentSettingsDialog::all_supported_color = {
    {tr("Red"), Qt::red},     {tr("Green"), Qt::green},
    {tr("Blue"), Qt::blue},   {tr("Yellow"), Qt::yellow},
    {tr("Black"), Qt::black}, {tr("White"), Qt::white}};

ComponentSettingsDialog::ComponentSettingsDialog(AbstractComponent *component,
                                                 QWidget *parent)
    : QDialog(parent), component_(component) {
  initPortsReader();
  initUi();
  initConnections();
}

ComponentSettingsDialog::~ComponentSettingsDialog() {}

void ComponentSettingsDialog::appendSettingWidget(QWidget *widget) {
  basic_settings_layout_->addWidget(widget);
}

void ComponentSettingsDialog::appendSettingLayout(QLayout *layout) {
  basic_settings_layout_->addLayout(layout);
}

void ComponentSettingsDialog::initPortsReader() {
  ports_file_reader_ = new rabbit_App::ports::PortsFileReader(this);
  component_->askForConstraintFilePath();
  // check if the file exists
  QFile file(component_->constraintFilePath());
  if (!file.exists()) {
    QMessageBox::warning(this, tr("Warning"), tr("Constraint file not found!"));
    return;
  }
  ports_file_reader_->readFromFile(component_->constraintFilePath());
}

void ComponentSettingsDialog::initUi() {
  auto type = component_->componentType();
  setWindowTitle(type + tr(" Settings"));
  setMinimumWidth(kWindowMinWidth);

  QLabel *basic_settings_label = new QLabel("Basic Settings", this);

  basic_settings_group_ = new QGroupBox(this);
  basic_settings_layout_ = new QVBoxLayout(basic_settings_group_);

  QLabel *component_name_label =
      new QLabel("Component Name: ", basic_settings_group_);
  component_name_edit_ =
      new QLineEdit(component_->componentName(), basic_settings_group_);
  auto name_layout = new QHBoxLayout();
  name_layout->addWidget(component_name_label);
  name_layout->addWidget(component_name_edit_);
  basic_settings_layout_->addLayout(name_layout);

  QLabel *port_settings_label = new QLabel("Port Settings", this);

  initTable();

  ok_button_ = new QPushButton(tr("OK"), this);
  cancel_button_ = new QPushButton(tr("Cancel"), this);

  auto main_layout = new QVBoxLayout(this);
  auto button_layout = new QHBoxLayout();
  button_layout->addWidget(ok_button_);
  button_layout->addWidget(cancel_button_);
  main_layout->addWidget(basic_settings_label);
  main_layout->addWidget(basic_settings_group_);
  main_layout->addWidget(port_settings_label);
  main_layout->addWidget(table_view_);
  main_layout->addLayout(button_layout);
  main_layout->setContentsMargins(10, 10, 10, 10);
}

void ComponentSettingsDialog::initConnections() {
  connect(ok_button_, &QPushButton::clicked, this,
          &ComponentSettingsDialog::accept);
  connect(cancel_button_, &QPushButton::clicked, this,
          &ComponentSettingsDialog::reject);
}

void ComponentSettingsDialog::initTable() {

  table_view_ = new QTableView(this);
  table_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  model_ = new QStandardItemModel(this);

  model_->setColumnCount(4);
  const QVector<QString> header_name = {tr("Component Port Name"),
                                        tr("Port Type"), tr("Pin Name"),
                                        tr("HDL Port Name")};
  for (int i = 0; i < header_name.size(); ++i) {
    model_->setHeaderData(i, Qt::Horizontal, header_name[i]);
  }
  table_view_->verticalHeader()->hide();

  table_view_->setModel(model_);

  auto *header = table_view_->horizontalHeader();
  header->setSectionResizeMode(QHeaderView::ResizeToContents);
  header->setMinimumWidth(80);
  header->setStretchLastSection(true);

  auto raw_component = component_->rawComponent();
  addRow(raw_component->inputPorts(), ports::PortType::Input);
  addRow(raw_component->outputPorts(), ports::PortType::Output);
}

void ComponentSettingsDialog::addRow(const QVector<ports::Port> &ports,
                                     ports::PortType type) {

  auto &choosable_ports = (type == ports::PortType::Input)
                              ? ports_file_reader_->inputs()
                              : ports_file_reader_->outputs();

  for (auto &port : ports) {
    int row_index = model_->rowCount();

    QList<QStandardItem *> row_items;
    row_items.append(createItem(port.name));
    row_items.append(createItem(ports::portTypeToString(port.type)));
    model_->appendRow(row_items);

    auto combobox = creatCombobox(choosable_ports);
    QString combobox_text;
    if (!port.pin_name.isEmpty() &&
        ports::declIndexMap(port.pin_name).second != -1) {
      combobox_text = findHdlPortName(choosable_ports, port.pin_name);
    } else {
      combobox_text = "None";
    }
    combobox->setCurrentText(combobox_text);
    original_ports_names_.push_back(combobox_text);
    auto pin_name_label = new QLabel(port.pin_name, this);
    pin_name_label->setStyleSheet("color:grey");
    pin_name_label->setAlignment(Qt::AlignCenter);
    table_view_->setIndexWidget(model_->index(row_index, 2), pin_name_label);
    table_view_->setIndexWidget(model_->index(row_index, 3), combobox);
    connect(combobox, &QComboBox::currentTextChanged, this,
            [=](const QString &text) {
              pin_name_label->setText(
                  ports_file_reader_->findPort(text, type).pin_name);
            });
  }
}

QComboBox *
ComponentSettingsDialog::creatCombobox(const QVector<ports::Port> &map) {
  auto combobox = new QComboBox(table_view_);

  for (auto &port : map) {
    combobox->addItem(port.name);
  }
  combobox->addItem(tr("None"));
  return combobox;
}

QStandardItem *ComponentSettingsDialog::createItem(const QString &text) {
  auto item = new QStandardItem(text);
  item->setTextAlignment(Qt::AlignCenter);
  item->setEditable(false);
  return item;
}

const QString
ComponentSettingsDialog::findHdlPortName(const QVector<ports::Port> &map,
                                         const QString &pin_name) {
  for (auto &port : map) {
    if (port.pin_name == pin_name) {
      return port.name;
    }
  }
  throw(std::runtime_error(
      "ComponentSettingsDialog::findHdlPortName: pin name not found"));
}

void ComponentSettingsDialog::accept() {
  auto raw_component = component_->rawComponent();
  auto &inputs_vec = raw_component->inputPorts();
  auto &outputs_vec = raw_component->outputPorts();
  for (int i = 0; i < model_->rowCount(); i++) {
    auto component_pname = model_->item(i, 0)->text();
    auto port_type = model_->item(i, 1)->text();
    auto pin_name =
        qobject_cast<QLabel *>(table_view_->indexWidget(model_->index(i, 2)))
            ->text();
    auto hdl_pname =
        qobject_cast<QComboBox *>(table_view_->indexWidget(model_->index(i, 3)))
            ->currentText();
    if (hdl_pname != original_ports_names_[i]) {
      is_modifieds_ = true;
    }
    bool is_none = (hdl_pname == tr("None"));
    switch (ports::stringToPortType(port_type)) {
    case ports::PortType::Input: {
      auto &port = inputs_vec[i];
      port.pin_name = is_none ? "" : pin_name;
      port.pin_index = ports::inputDeclIndexMap(pin_name);
      break;
    }
    case ports::PortType::Output: {
      auto &port_2 = outputs_vec[i];
      port_2.pin_name = is_none ? "" : pin_name;
      port_2.pin_index = ports::outputDeclIndexMap(pin_name);
      break;
    }
    default:
      throw(std::runtime_error(
          "ComponentSettingsDialog::accept: port type error"));
    }
  }
  if (component_->componentName() != component_name_edit_->text()) {
    component_->setComponentName(component_name_edit_->text());
    is_modifieds_ = true;
  }
  acceptDerivedClassSettings();
  if (is_modifieds_) {
    component_->settingsChanged();
  }
  QDialog::accept();
}

ActiveModeSettingsDialog::ActiveModeSettingsDialog(AbstractComponent *component,
                                                   QWidget *parent)
    : ComponentSettingsDialog(component, parent) {
  QGroupBox *active_mode_group_box = new QGroupBox(tr("Active Mode"), this);
  QHBoxLayout *active_mode_layout = new QHBoxLayout(active_mode_group_box);
  active_high_radio_button_ = new QRadioButton(tr("High Active"), this);
  active_high_radio_button_->setChecked(
      !component->rawComponent()->isLowActive());
  QRadioButton *active_low_radio_button_ =
      new QRadioButton(tr("Low Active"), this);
  active_low_radio_button_->setChecked(
      component->rawComponent()->isLowActive());
  active_mode_layout->addWidget(active_high_radio_button_);
  active_mode_layout->addWidget(active_low_radio_button_);
  appendSettingWidget(active_mode_group_box);
}

ActiveModeSettingsDialog::~ActiveModeSettingsDialog() {}

void ActiveModeSettingsDialog::acceptDerivedClassSettings() {
  if (active_high_radio_button_->isChecked() ==
      component_->rawComponent()->isLowActive()) {
    is_modifieds_ = true;
    component_->rawComponent()->setLowActive(
        !active_high_radio_button_->isChecked());
  }
}

VisionPersistenceSettingsDialog::VisionPersistenceSettingsDialog(
    AbstractComponent *component, QWidget *parent)
    : ComponentSettingsDialog(component, parent) {
  vision_persistence_edit_ = new QLineEdit(this);
  vision_persistence_edit_->setValidator(
      new QRegularExpressionValidator(QRegularExpression("[0-9]{1,4}")));
  vision_persistence_edit_->setText(
      QString::number(component_->rawComponent()->visionPersistence()));
  QHBoxLayout *vision_persistence_layout = new QHBoxLayout();
  vision_persistence_layout->addWidget(
      new QLabel(tr("Vision Persistence: "), this));
  vision_persistence_layout->addWidget(vision_persistence_edit_);
  vision_persistence_layout->addWidget(new QLabel("ms", this));
  appendSettingLayout(vision_persistence_layout);
}

VisionPersistenceSettingsDialog::~VisionPersistenceSettingsDialog() {}

void VisionPersistenceSettingsDialog::acceptDerivedClassSettings() {
  auto text = vision_persistence_edit_->text();
  auto vision_persistence = vision_persistence_edit_->text().toUInt();
  if (static_cast<int>(vision_persistence) !=
      component_->rawComponent()->visionPersistence()) {
    is_modifieds_ = true;
    component_->rawComponent()->setVisionPersistence(vision_persistence);
  }
}

ColorSettingsDialog::ColorSettingsDialog(AbstractComponent *component,
                                         QWidget *parent)
    : ComponentSettingsDialog(component, parent) {
  QGroupBox *colors_group_box = new QGroupBox(tr("Colors"), this);
  QGridLayout *colors_layout = new QGridLayout(colors_group_box);
  auto &supported_colors = component_->rawComponent()->supportedColors();
  auto &colors_map = component_->rawComponent()->componentColors();
  for (auto itor = colors_map.begin(); itor != colors_map.end(); ++itor) {
    auto color_usage = itor.key();
    auto color = itor.value();
    QLabel *color_label = new QLabel(color_usage, this);
    QComboBox *color_combo_box = new QComboBox(this);
    for (auto supported_color : supported_colors) {
      color_combo_box->addItem(
          component_->rawComponent()->supportedColors().key(supported_color));
    }
    color_combo_box->setCurrentText(
        component_->rawComponent()->supportedColors().key(color));
    color_map_[color_usage] = color_combo_box;
    colors_layout->addWidget(color_label, colors_layout->rowCount(), 0);
    colors_layout->addWidget(color_combo_box, colors_layout->rowCount() - 1, 1);
  }
  appendSettingWidget(colors_group_box);
}

ColorSettingsDialog::~ColorSettingsDialog() { color_map_.clear(); }

void ColorSettingsDialog::acceptDerivedClassSettings() {
  auto &colors_map = component_->rawComponent()->componentColors();
  for (auto itor = colors_map.begin(); itor != colors_map.end(); ++itor) {
    auto color_usage = itor.key();
    auto color_combo_box = color_map_.value(color_usage);
    if (colors_map[color_usage] !=
        ComponentSettingsDialog::all_supported_color.value(
            color_combo_box->currentText())) {
      is_modifieds_ = true;
      colors_map[color_usage] =
          ComponentSettingsDialog::all_supported_color.value(
              color_combo_box->currentText());
    }
  }
}
