#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QList>
#include <QString>

/*
 * This file is used to include all the components.
 * The `IWYU pragma: export' is used to tell IWYU to export the included
 * headers. I declared the pragma to disable the warning.
 */

#include "ButtonComponent.h"         // IWYU pragma: export
#include "GraphicLCDComponent.h"     // IWYU pragma: export
#include "KeyPadComponent.h"         // IWYU pragma: export
#include "LEDComponent.h"            // IWYU pragma: export
#include "LEDMatrixComponent.h"      // IWYU pragma: export
#include "RotaryButtonComponent.h"   // IWYU pragma: export
#include "SegmentDisplayComponent.h" // IWYU pragma: export
#include "SwitchComponent.h"         // IWYU pragma: export
#include "TextLCDComponent.h"        // IWYU pragma: export
#include "PS2KeyboardComponent.h"

// add new input component here
inline QList<QString> inputComponents() {
  return QList<QString>{"Switch", "Button", "KeyPad", "SmallKeyPad",
                        "RotaryButton", "PS2Keyboard"};
}

// add new output conponent here
inline QList<QString> outputComponents() {
  return QList<QString>{"LED",
                        "TextLCD",
                        "GraphicLCD",
                        "SegmentDisplay",
                        "FourDigitSegmentDisplay",
                        "LED4x4Matrix",
                        "LED8x8Matrix",
                        "LED16x16Matrix"};
}

#endif // COMPONENTS_H
