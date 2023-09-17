#pragma once
#ifndef PORT_H
#define PORT_H

#include <QString>

namespace rabbit_App::ports {

enum class PortType { Input, Output, Clock, Undefined };
const QString portTypeToString(const PortType type);
const PortType stringToPortType(const QString &type);

/// @brief Port struct
/// @details This struct is used to store the information of a port.
/// @param name The name of the port. aka the name you defined in HDL.
/// @param pin_name The name of the pin. e.g "P20"
/// @param pin_index The index of the pin. Every pin has a unique index, which
/// can be get from the pin name.
/// @param type The type of the port. e.g input, output, clock.
struct Port {
  Port(const QString &name, const QString &pin_name, int pin_index,
       PortType type)
      : name(name), pin_name(pin_name), pin_index(pin_index), type(type) {}
  Port() : name(""), pin_name(""), pin_index(-1), type(PortType::Undefined) {}

  QString name;
  QString pin_name;
  int pin_index;
  PortType type;
};

} // namespace rabbit_App::ports

#endif // PORT_H
