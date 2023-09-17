# Rabbit Components Development Documentation

## Content
[TOC]

## Introduction

The Components part of Rabbit is responsible for the display using the data received from the FPGA board. It contains two parts: the component panel and various components. This is how they work:

![Components](./doc/images/Components.png)

The source code of the components part is in `rabbit_App\src\Components`.

## Components Panel

### Introduction

Component panel is the place where the components are shown. When the button of a component in the toolbar is clicked, a signal will be emitted from toolbar to main window, and then the main window will emit a signal to the component panel to add the component. Also, component panel is the middleman between the VLFDDeviceHandler and the components. When the FPGA board is running, the component panel will get the data from the VLFDDeviceHandler and send it to the components, get the data from the components and send it to the VLFDDeviceHandler in the meantime.

The source code of the component panel is in `rabbit_App\src\Components\ComponentsPanel.cpp`. We use a class `ComponentsPanel` to implement the panel. 

### Functions & Members

Here is the main functions of the class:

* `uint64_t getWriteData() const` : Get the data to be written to the FPGA board. This function will be called by the main window when write data is requested in VLFDRunning

* `appendComponent(AbstractComponent *component)` : Add a component to the panel. This function will be called when the buttons on the component page are clicked.

* `resetAllComponents()` : Reset all the components.

* `dragEnterEvent(QDragEnterEvent *event)` `dragMoveEvent(QDragMoveEvent *event)` `dropEvent(QDropEvent *event)` : These functions are used to implement the drag and drop of the component tiles.

The only member variable of the class is `QHash<QUuid, AbstractComponent *> components`, which is a hash table that stores the current components. The key is the UUID of the component, and the value is the pointer to the component.

## Component

### Introduction

There are several classes related to the components.
* `AbstractComponent` : The base class of all the components. It is an abstract class, so it cannot be instantiated. It contains the basic functions and members of a component.
* 



### Functions & Members


## How to Add a New Component