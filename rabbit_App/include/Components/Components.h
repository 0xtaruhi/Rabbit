#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include <QColor>
#include <QLabel>
#include <QWidget>

#define LED_RAW_EXTRA_MEMBERS                                                  \
  QColor color_ = Qt::red;                                                     \
  QLabel *led_picture_ = nullptr;

namespace rabbit_App::component {
COMPONENT_CLASS_DECLARATION(Output, LED)
} // namespace rabbit_App::component

#endif // COMPONENTS_H
