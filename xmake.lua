-- Rabbit FPGA Development Tool - Xmake Build Configuration

set_project("Rabbit")
set_version("1.2.0")
set_description("FPGA Development Tool with Component Library")

add_rules("mode.debug", "mode.release")
set_languages("c++17")
set_warnings("all")

includes("rabbit_App/3rdparty/TabToolbar")

-- Platform-specific compiler flags
if is_plat("windows") then
    add_cxxflags("/W3", "/utf-8", "/Zc:__cplusplus", "/EHsc")
    add_cxxflags("/D_CRT_SECURE_NO_WARNINGS")
    add_defines("WIN32_LEAN_AND_MEAN", "NOMINMAX")
else
    add_cxxflags("-Wall", "-Werror", "-Wextra")
    add_cxxflags("-fPIC")
end

add_requires("libusb")

-- Main application target
target("rabbit_App")
    add_rules("qt.application")
    set_kind("binary")
    set_default(true)
    
    add_defines("RABBIT_APP")
    
    add_files("rabbit_App/src/**.cpp")
    add_files("rabbit_App/include/**.h")
    add_files("rabbit_App/res/*.qrc")
    add_files("rabbit_App/3rdparty/TabToolbar/src/TabToolbar/*.qrc")
    
    add_frameworks("QtWidgets", "QtGui", "QtCore")
    
    add_includedirs("rabbit_App/include")
    add_includedirs("vlfd-ffi")
    
    add_deps("TabToolbar")
    add_packages("libusb")
    
    add_links("vlfd_ffi")
    add_linkdirs("vlfd-ffi/target/release")
    
    if is_plat("windows") then
        add_syslinks("userenv", "ntdll", "kernel32", "advapi32")
    end

    before_build(function (target)
        print("[rabbit] before_build: build vlfd_ffi")
        os.exec("cargo build --manifest-path vlfd-ffi/Cargo.toml --release")
    end)
    
    after_build(function (target)
        local build_dir = "$(builddir)/$(plat)/$(arch)/$(mode)"
        local gtkwave_dir = is_plat("macosx") and build_dir .. "/gtkwave.app" or build_dir .. "/gtkwave"
        
        if not os.isdir(gtkwave_dir) then
            local archive = is_plat("macosx") and (is_arch("arm64") and "gtkwave/osx-arm64/gtkwave.zip" or "gtkwave/osx-x64/gtkwave.zip")
                    or is_plat("windows") and "gtkwave/win-x64/gtkwave.zip"
                    or "gtkwave/linux-x64/gtkwave.tar.gz"
            
            os.cp(archive, build_dir .. "/")
            os.cd(build_dir)
            os.exec("tar -xf " .. path.filename(archive))
            os.rm(path.filename(archive))
        end
    end)