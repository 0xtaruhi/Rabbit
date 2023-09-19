#pragma once
#ifndef PIN_INFO_H
#define PIN_INFO_H

#include <QPair>
#include <QString>


#include "Port.h"

namespace rabbit_App::ports {

auto inputDeclIndexMap(const QString &pin_name) -> int;
auto outputDeclIndexMap(const QString &pin_name) -> int;

auto declIndexMap(const QString &pin_name) -> QPair<PortType, int>;

auto getClockPinDecl() -> QString;

} // namespace rabbit_App::ports

#endif // PIN_INFO_H
