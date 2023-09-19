# Rabbit Development Documentation

## Content

- [Rabbit Development Documentation](#rabbit-development-documentation)
  - [Content](#content)
  - [Introduction](#introduction)
  - [Main Window](#main-window)
    - [Toolbar](#toolbar)
    - [Components Panel](#components-panel)
    - [Prompt Bar](#prompt-bar)
  - [FPGA Communication](#fpga-communication)
  - [Component](#component)

## Introduction


## Main Window

The main window is the main interface of Rabbit. It is composed of a toolbar, a components panel, and a prompt bar.
The source code of the main window is in `rabbit_App\src`

### Toolbar
We use a 3rd party library ... to implement the toolbar. The source code of the toolbar is in `rabbit_App\3rdparty\TabToolbar`.
We also package a class `MainTabToolBar` to custom it. The source code of the class is in `rabbit_App\src\MainTabToolBar.cpp`.

### Components Panel

Component panel is the place where the components are shown. The source code of the component panel is in `rabbit_App\src\Components\ComponentsPanel.cpp`. We use a class `ComponentsPanel` to implement the panel.

When the button of a component in the toolbar is clicked, a signal will be emitted from toolbar to main window, and then the main window will emit a signal to the component panel to add the component.




### Prompt Bar

## FPGA Communication


## Component