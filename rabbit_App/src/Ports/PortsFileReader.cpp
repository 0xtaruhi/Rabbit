#include <QFile>
#include <QXmlStreamReader>

#include "Ports/PinInfo.h"
#include "Ports/PortsFileReader.h"

using namespace rabbit_App::ports;

PortsFileReader::PortsFileReader(QObject *parent) : QObject(parent) {
  inputs_.reserve(53);
  outputs_.reserve(53);
}

PortsFileReader::~PortsFileReader() {}

const Port &PortsFileReader::findPort(const QString &port_name,
                                      PortType type) const {
  if (type == PortType::Input) {
    return inputs_[inputs_order_[port_name]];
  } else if (type == PortType::Output) {
    return outputs_[outputs_order_[port_name]];
  } else {
    return clock_;
  }
}

void PortsFileReader::readFromFile(const QString &file_path) {
  QFile file = QFile(file_path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Could not open file");
  }

  QXmlStreamReader xml_reader(&file);
  while (!xml_reader.atEnd() && !xml_reader.hasError()) {
    const auto &token_type = xml_reader.readNext();
    if (token_type == QXmlStreamReader::StartElement) {
      const auto ele_name = xml_reader.name().toString();
      if (ele_name == "port") {
        const auto port_name = xml_reader.attributes().value("name").toString();
        const auto pin_name =
            xml_reader.attributes().value("position").toString();

        auto [type, index] = declIndexMap(pin_name);
        if (type == PortType::Input) {
          inputs_.emplace_back(port_name, pin_name, index, type);
          inputs_order_.insert(port_name, inputs_.size() - 1);
          // inputs_.emplace(port_name, port_name, pin_name, index, type);
        } else if (type == PortType::Output) {
          outputs_.emplace_back(port_name, pin_name, index, type);
          outputs_order_.insert(port_name, outputs_.size() - 1);
          // outputs_.emplace(port_name, port_name, pin_name, index, type);
        } else if (type == PortType::Clock) {
          clock_ = Port(port_name, pin_name, index, type);
        }
      }
    }
  }

  if (xml_reader.hasError()) {
    throw std::runtime_error("XML Error: " +
                             xml_reader.errorString().toStdString());
  }
}
