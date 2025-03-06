add_rules("mode.debug", "mode.release")
set_languages("c++17")

includes("rabbit_App/3rdparty/TabToolbar")
if is_plat("windows") then
    add_cxxflags("/W3", "/utf-8", "/Zc:__cplusplus", "/EHsc")
    add_cxxflags("/D_CRT_SECURE_NO_WARNINGS")
else
    add_cxxflags("-Wall", "-Werror")
end

-- target("VLFDLibUSB")
--     set_kind("static")
--     add_files("VLFDLibUSBDriver/*.cpp")
--     add_includedirs("VLFDLibUSBDriver", {public = true})
--     add_includedirs("libusb/libusb")
--     add_links("usb-1.0")
--     add_linkdirs("build", "libusb/libusb/.libs")

-- target("rabbit_VLFD")
--     set_kind("static")
--     add_files("rabbit_VLFD/src/*.cpp")
--     add_includedirs("rabbit_VLFD/include", {public = true})
--     add_includedirs("libusb/libusb")
--     add_links("usb-1.0")
--     add_linkdirs("libusb/libusb/.libs")
--     add_deps("VLFDLibUSB")

target("rabbit_App")
    add_rules("qt.application")
    set_kind("binary")
    add_defines("RABBIT_APP")
    add_files("rabbit_App/src/**.cpp")
    add_files("rabbit_App/include/**.h")
    add_files("rabbit_App/res/*.qrc")
    add_files("rabbit_App/3rdparty/TabToolbar/src/TabToolbar/*.qrc")
    add_frameworks("QtWidgets", "QtGui", "QtCore")
    add_includedirs("rabbit_App/include")
    add_includedirs("libusb/libusb")
    add_includedirs("VLFDLibUSBDriver")
    add_links("usb-1.0")
    if is_plat("macosx") then
        if is_arch("arm64") then
            add_links("VLFDLibUSB-macos-arm64")
        else
            add_links("VLFDLibUSB-macos-x64")
        end
    elseif is_plat("windows") then
        add_links("VLFDLibUSB-win-x64")
    elseif is_plat("linux") then
        add_links("VLFDLibUSB-linux-x64")
    end
    -- add_links("VLFDLibUSB")
    add_linkdirs("libusb/libusb/.libs")
    add_linkdirs("VLFDLibUSBDriver")
    -- add_deps("rabbit_VLFD", "TabToolbar")
    -- add_deps("VLFDLibUSB", "TabToolbar")
    add_deps("TabToolbar")
    after_build(function (target)
        if is_plat("macosx") then
            -- print("macosx")
            if is_arch("arm64") then
                -- print("macosx-arm64")
                if not os.isdir("$(buildir)/$(plat)/$(arch)/$(mode)/gtkwave.app") then
                    -- print("macosx gtkwave")
                    os.cp("gtkwave/osx-arm64/gtkwave.zip", "$(buildir)/$(plat)/$(arch)/$(mode)/")
                    os.cd("$(buildir)/$(plat)/$(arch)/$(mode)/")
                    os.exec("tar -xf gtkwave.zip")
                    os.rm("gtkwave.zip")
                end
            else
                if not os.isdir("$(buildir)/$(plat)/$(arch)/$(mode)/gtkwave.app") then
                    -- print("macosx gtkwave")
                    os.cp("gtkwave/osx-x64/gtkwave.zip", "$(buildir)/$(plat)/$(arch)/$(mode)/")
                    os.cd("$(buildir)/$(plat)/$(arch)/$(mode)/")
                    os.exec("tar -xf gtkwave.zip")
                    os.rm("gtkwave.zip")
                end
            end
        elseif is_plat("windows") then
            if not os.isdir("$(buildir)/$(plat)/$(arch)/$(mode)/gtkwave") then
                os.cp("gtkwave/win-x64/gtkwave.zip", "$(buildir)/$(plat)/$(arch)/$(mode)/")
                os.cd("$(buildir)/$(plat)/$(arch)/$(mode)/")
                os.exec("tar -xf gtkwave.zip")
                os.rm("gtkwave.zip")
            end
        elseif is_plat("linux") then 
            if not os.isdir("$(buildir)/$(plat)/$(arch)/$(mode)/gtkwave") then
                os.cp("gtkwave/linux-x64/gtkwave.tar.gz", "$(buildir)/$(plat)/$(arch)/$(mode)/")
                os.cd("$(buildir)/$(plat)/$(arch)/$(mode)/")
                os.exec("tar -xf gtkwave.tar.gz")
                os.rm("gtkwave.tar.gz")
            end
        end
    end)

