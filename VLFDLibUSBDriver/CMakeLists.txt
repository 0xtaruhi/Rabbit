cmake_minimum_required(VERSION 3.10)

project(VLFDLibUSBDriver)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
    set(arch "x64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
    set(arch "x86")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "armhf")
    set(arch "armhf")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
    set(arch "arm64")
endif()

if(WIN32)
	set(myos "win")
elseif(APPLE)
	set(myos "macos")
elseif(UNIX)
	set(myos "linux")
endif()

set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_C_COMPILER gcc)
#set(CMAKE_CXX_COMPILER gcc)

set(libusbdir ${PROJECT_SOURCE_DIR}/../libusb)

set(LIBUSB_CC ${CMAKE_C_COMPILER})
set(LIBUSB_CXX ${CMAKE_CXX_COMPILER})

execute_process(
	COMMAND chmod +x autogen.sh WORKING_DIRECTORY ${libusbdir}
	COMMAND chmod +x bootstrap.sh WORKING_DIRECTORY ${libusbdir}
	COMMAND sh autogen.sh --with-pic --disable-udev WORKING_DIRECTORY ${libusbdir}
)

add_custom_target(LIBUSB
                COMMAND make WORKING_DIRECTORY ${libusbdir}
)

set(srcs 
    pch.cpp
    SMIMS_VLFD.cpp 
    ProgramVLFD.cpp 
    SMIMS_iobase.cpp 
    SMIMS_API.cpp 
    serialcheck.cpp
)

set(headers
    SMIMS_define.h
)

include_directories(${PROJECT_SOURCE_DIR}/../libusb/libusb)
link_directories(${PROJECT_SOURCE_DIR}/../libusb/libusb/.libs)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../Native/${myos}/${arch}/)

add_library(VLFD STATIC ${srcs} ${headers})
add_dependencies(VLFD LIBUSB)

add_library(libusb STATIC IMPORTED)
set_property(TARGET libusb PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../libusb/libusb/.libs/libusb-1.0.a)

if(APPLE)
target_link_libraries(VLFD libusb "-framework CoreFoundation" "-framework IOKit" objc)
else()
target_link_libraries(VLFD libusb)
endif()
