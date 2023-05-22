add_rules("mode.debug", "mode.release")
set_languages("c++17")

add_subdirs("rabbit_App/3rdparty/TabToolbar")
if is_plat("windows") then
    add_cxxflags("/W3", "/utf-8")
else
    add_cxxflags("-Wall", "-Werror")
end

target("VLFDLibUSB")
    set_kind("static")
    add_files("VLFDLibUSBDriver/*.cpp")
    add_includedirs("VLFDLibUSBDriver", "libusb/libusb")
    add_links("usb-1.0")
    add_linkdirs("build", "libusb/libusb/.libs")

target("rabbit_VLFD")
    set_kind("static")
    add_files("rabbit_VLFD/src/*.cpp")
    add_includedirs("rabbit_VLFD/include", "VLFDLibUSBDriver", "libusb/libusb")
    add_links("usb-1.0")
    add_linkdirs("libusb/libusb/.libs")
    add_deps("VLFDLibUSB")

target("rabbit_App")
    add_rules("qt.application")
    set_kind("binary")
    add_files("rabbit_App/src/**.cpp")
    add_files("rabbit_App/include/**.h")
    add_files("rabbit_App/res/*.qrc")
    add_files("rabbit_App/3rdparty/TabToolbar/src/TabToolbar/*.qrc")
    add_frameworks("QtWidgets", "QtGui", "QtCore")
    add_includedirs("rabbit_App/include", "rabbit_VLFD/include")
    add_includedirs("rabbit_App/3rdparty/TabToolbar/include")
    add_deps("rabbit_VLFD", "TabToolbar")
