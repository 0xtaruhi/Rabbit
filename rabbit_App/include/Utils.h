#pragma once
#include "qpixmap.h"
#ifndef UTILS_H
#define UTILS_H

#include <QPixmap>

namespace rabbit_App {

/// @brief Returns a semi-transparent pixmap.
/// @param pixmap The pixmap to make semi-transparent.
/// @param alpha The alpha value to use.
/// @return A semi-transparent pixmap.
QPixmap getTransparentPixmap(const QPixmap &pixmap, ushort alpha);

/// @brief Sets the pixmap to be semi-transparent.
/// @param pixmap The pixmap to make semi-transparent.
/// @param alpha The alpha value to use.
void setTransparentPixmap(QPixmap &pixmap, ushort alpha);

} // namespace rabbit_App

#endif // UTILS_H
