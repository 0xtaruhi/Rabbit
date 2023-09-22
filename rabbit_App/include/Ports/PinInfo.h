#pragma once
#ifndef PIN_INFO_H
#define PIN_INFO_H

#include <QPair>
#include <QString>

#include "Port.h"

namespace rabbit_App::ports {

/// @brief Get the index of the pin in the input port.
auto inputDeclIndexMap(const QString &pin_name) -> int;

/// @brief Get the index of the pin in the output port.
auto outputDeclIndexMap(const QString &pin_name) -> int;

/// @brief Get the index of the pin in the port.
auto declIndexMap(const QString &pin_name) -> QPair<PortType, int>;

auto getClockPinDecl() -> QString;

} // namespace rabbit_App::ports

#endif // PIN_INFO_H
