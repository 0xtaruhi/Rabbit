# Rabbit Virtual Components Platform

## Tabla de Contenidos

- [Acerca de](#about)
- [Instalación](#installing)
- [Uso](#usage)
- [Desarrollo](#development)
- [Agregar Nuevo Componente](#add_new_component)
- [Problemas](#issues)
- [Colaboradores](#contributors)

## Acerca de <a name = "about"></a>

Rabbit es una plataforma de componentes virtuales para la placa FPGA FDP3P7. 
Puede recibir y enviar datos desde/hacia la placa FPGA a través de USB y mostrar los datos mediante una interfaz gráfica (GUI). 

[Wonton](https://github.com/ChayCai/Wonton_master) es la versión anterior de Rabbit, la cual fue implementada en C#(core) y Electron(GUI). Rabbit ha sido completamente reescrito en C++ y Qt6 para mejorar el rendimiento y la estabilidad.

## Instalación <a name= "installing"></a>

### 1. Compilación del Código Fuente e Instalación

Estas instrucciones le permitirán obtener una copia del proyecto ejecutándose en su máquina local para fines de desarrollo y prueba. 

#### Requisitos Previos

Antes de compilar Rabbit, necesita instalar [Qt6](https://www.qt.io/product/qt6) y [xmake](https://xmake.io/#/). 

#### Clonar el repositorio

```bash
git clone https://github.com/0xtaruhi/Rabbit.git
# git submodule update --init --recursive
```

#### Compilar libusb

Después de clonar el repositorio, primero debe compilar libusb.

```bash
git submodule update --init --recursive libusb
```

* Linux & MacOS

    ```bash
    cd libusb
    ./autogen.sh
    ./configure
    make
    ```

* Windows

    Abra `libusb\msvc\libusb.sln` con **Visual Studio** y compile la **biblioteca estática (static library)**. Luego copie `libusb\msvc\libusb\Release\libusb-1.0.lib` a `libusb\libusb\.libs\usb-1.0.lib`.

#### Nota: Si está en MacOS, compile y enlace la versión recién compilada de QT6.5.5 (o puede descargar los binarios precompilados)

##### Descargar QT6.5.5 precompilado para MacOS (arm64):

[Descargar aquí](https://github.com/jwwang2003/qt-everywhere-src-6.5.5/releases/tag/1.0.0)

Extraiga los archivos en la raíz del directorio de su proyecto. Tenga en cuenta que al configurar la ruta de QT con `xmake f`, debe elegir la carpeta `binaries` en lugar de la proporcionada (esa es en caso de que haya clonado el código fuente y compilado desde cero).

```bash
xmake f --qt=[path to binaries folder] -v
```

##### Compilar QT6.5.5 para MacOS (arm64):

Siga los pasos en el submódulo [qt-everywhere-src-6.5.5](./qt-everywhere-src-6.5.5/README.md) para descargar y compilar QT 6.5.5.

```bash
git submodule update --init --recursive qt-everywhere-src-6.5.5
```

Después de compilar QT/descargar los binarios de QT, siga los siguientes pasos (asegúrese de hacer `cd` de regreso al directorio raíz)

```bash
xmake f --qt=qt-everywhere-src-6.5.5/qt5/qtbase -v
```

#### Compilar y Ejecutar Rabbit

Luego puede compilar Rabbit.

```bash
cd ..
xmake build
```

Finalmente, puede ejecutar Rabbit.

```bash
xmake run
```

### 2. Instalación de Binarios

Consulte la [Página de Releases](https://github.com/0xtaruhi/Rabbit/releases) para descargar el paquete binario.

## Uso <a name = "usage"></a>

Consulte el [Manual de Usuario](./doc/RabbitHelpDoc.md) para más información.

## Desarrollo <a name = "development"></a>

Consulte el [Manual del Desarrollador](./doc/RabbitDevDoc.md) para más información.

## Agregar Nuevo Componente <a name = "add_new_component"></a>

Consulte el [Manual para Agregar Nuevo Componente](./doc/AddNewComponent.md) para más información.

## Problemas <a name = "issues"></a>

Si tiene alguna pregunta o sugerencia, no dude en abrir un "issue".

Aquí hay algunos problemas conocidos:

1. `libusb` no es compatible con el chip **M1** de MacOS. 

2. En ubuntu/debian, es necesario instalar `libxcb-cursor`:
   ```
   sudo apt-get install libxcb-cursor0 
   ``` 
   cuando ejecute el paquete binario y obtenga el siguiente mensaje de error:
   > /usr/local/bin/Rabbit/rabbit_App: error while loading shared libraries: libQt6Widgets.so.6: cannot open shared object file: No such file or directory


## Colaboradores <a name = "contributors"></a>

* [0xtaruhi](https://github.com/0xtaruhi) - Desarrollador Principal
* [Mxcln](https://github.com/Mxcln) - Desarrollador
