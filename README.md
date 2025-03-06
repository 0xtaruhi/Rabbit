# Rabbit Virtual Compnents Platform

## Table of Contents

- [About](#about)
- [Installing](#installing)
- [Usage](#usage)
- [Development](#development)
- [Add New Component](#add_new_component)
- [Issues](#issues)
- [Contributors](#contributors)

## About <a name = "about"></a>

Rabbit is a virtual components platform for FDP3P7 FPGA board. 
It can receive and send data from/to the FPGA board via USB and show the data through the GUI. 

[Wonton](https://github.com/ChayCai/Wonton_master) is the previous version of Rabbit which is implemented in C#(core) and Electron(GUI). Rabbit is completely rewritten in C++ and Qt6 to improve the performance and stability.

## Installing <a name= "installing"></a>

### 1. Source Code compilation and Installation

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

#### Prerequisites

Before building Rabbit, you need to install [Qt6](https://www.qt.io/product/qt6) and [xmake](https://xmake.io/#/). 

#### Clone the repository

```bash
git clone https://github.com/0xtaruhi/Rabbit.git
git submodule update --init --recursive
```

#### Build libusb

After cloning the repository, you need to build libusb first.

* Linux & MacOS

    ```bash
    cd libusb
    ./autogen.sh
    ./configure
    make
    ```

* Windows

    Open the `libusb\msvc\libusb.sln` with **Visual Studio** and build the **static library**. Then copy the `libusb\msvc\libusb\Release\libusb-1.0.lib` to `libusb\libusb\.libs\usb-1.0.lib`.

#### Build & Run Rabbit

##### Aside: If on MacOS, build & link the newly compiled QT6.5.5

Follow the steps in the submodule [qt-everywhere-src-6.5.5](./qt-everywhere-src-6.5.5/README.md) to download and build QT 6.5.5.

After building QT, follow the next steps (make sure to `cd` back to this root directory)

```bash
xmake f --qt=qt-everywhere-src-6.5.5/qt5/qtbase -v
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

### 2. Binary Installation

Refer to the [Release Page](https://github.com/0xtaruhi/Rabbit/releases) to download the binary package.

## Usage <a name = "usage"></a>

Refer to the [User Manual](./doc/RabbitHelpDoc.md) for more information.

## Development <a name = "development"></a>

Refer to the [Developer Manual](./doc/RabbitDevDoc.md) for more information.

## Add New Component <a name = "add_new_component"></a>

Refer to the [Add New Component Manual](./doc/AddNewComponent.md) for more information.

## Issues <a name = "issues"></a>

If you have any questions or suggestions, please feel free to open an issue.

Here are some known issues:

1. `libusb` does not support MacOS **M1** chip. 

2. On ubuntu/debian, you need to install `libxcb-cursor` :
   ```
   sudo apt-get install libxcb-cursor0 
   ``` 
   when you run the binary package and get the error message:
   > /usr/local/bin/Rabbit/rabbit_App: error while loading shared libraries: libQt6Widgets.so.6: cannot open shared object file: No such file or directory


## Contributors <a name = "contributors"></a>

* [0xtaruhi](https://github.com/0xtaruhi) - Main Developer
* [Mxcln](https://github.com/Mxcln) - Developer