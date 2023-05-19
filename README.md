# Rabbit Virtual Compnents Platform

## Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Usage](#usage)
- [Contributing](../CONTRIBUTING.md)

## About <a name = "about"></a>

Rabbit is a virtual components platform for FDP3P7 FPGA boad. 
It can receive and send data from/to the FPGA board via USB and show the data through the GUI. 

[Wonton](https://github.com/ChayCai/Wonton_master) is the previous version of Rabbit which is implemented in C#(core) and Electron(GUI). Rabbit is completely rewritten in C++ and Qt6 to improve the performance and stability.

## Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites

Before building Rabbit, you need to install [Qt6](https://www.qt.io/product/qt6) and [xmake](https://xmake.io/#/). 

### Installing

```bash
git clone https://github.com/0xtaruhi/Rabbit.git
git submodule update --init --recursive
```

After cloning the repository, you need to build libusb first.

```bash
cd libusb
./autogen.sh
./configure
make
```

Then you can build Rabbit.

```bash
cd ..
xmake build
```

Finally, you can run Rabbit.

```bash
xmake run
```

## Usage <a name = "usage"></a>

The project is still under development. I will update the usage when the project is ready to use.
