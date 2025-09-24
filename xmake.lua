add_rules("mode.debug", "mode.release")
set_languages("c++17")

includes("rabbit_App/3rdparty/TabToolbar")
if is_plat("windows") then
    add_cxxflags("/W3", "/utf-8", "/Zc:__cplusplus", "/EHsc")
    add_cxxflags("/D_CRT_SECURE_NO_WARNINGS")
else
    add_cxxflags("-Wall", "-Werror")
end

add_requires("libusb")

local function is_debug_mode()
    return is_mode("debug")
end

local function rust_mode_dir()
    return is_debug_mode() and "debug" or "release"
end

local function rust_staticlib_path()
    return string.format("vlfd-ffi/target/%s/libvlfd_ffi.a", rust_mode_dir())
end

local function ensure_rust_ffi_built()
    -- Only check if the Rust static library exists; actual build happens in vlfd_ffi target
    return os.isfile(rust_staticlib_path())
end

target("vlfd_ffi")
    set_kind("phony")
    on_load(function (target)
        target:add("includedirs", "vlfd-ffi")
        local libdir = string.format("vlfd-ffi/target/%s", rust_mode_dir())
        local libpath = rust_staticlib_path()
        if os.isfile(libpath) then
            target:add("linkdirs", libdir)
            target:add("links", "vlfd_ffi")
        end
    end)
    on_build(function (target)
        if os.isfile("vlfd-ffi/Cargo.toml") then
            local args = "--manifest-path vlfd-ffi/Cargo.toml"
            if not is_debug_mode() then
                args = args .. " --release"
            end
            os.run("cargo build %s", args)
        else
            print("[vlfd_ffi] skip: vlfd-ffi/Cargo.toml not found")
        end
        local libdir = string.format("vlfd-ffi/target/%s", rust_mode_dir())
        if os.isdir(libdir) then
            target:add("linkdirs", libdir)
            target:add("links", "vlfd_ffi")
        end
    end)

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
    add_includedirs("vlfd-ffi")
    
    add_deps("TabToolbar")
    add_deps("vlfd_ffi", {inherit = true})
    add_packages("libusb")

    on_load(function (target)
        -- Ensure Rust static lib is built and linked explicitly
        if ensure_rust_ffi_built() then
            local libdir = string.format("vlfd-ffi/target/%s", rust_mode_dir())
            if os.isdir(libdir) then
                target:add("linkdirs", libdir)
                target:add("links", "vlfd_ffi")
                target:add("includedirs", "vlfd-ffi")
            end
        end
    end)

    after_build(function (target)
        print("[rabbit] after_build: unpack gtkwave if needed")
        if is_plat("macosx") then
            local app_dir = "$(builddir)/$(plat)/$(arch)/$(mode)/gtkwave.app"
            if not os.isdir(app_dir) then
                if is_arch("arm64") then
                    os.cp("gtkwave/osx-arm64/gtkwave.zip", "$(builddir)/$(plat)/$(arch)/$(mode)/")
                else
                    os.cp("gtkwave/osx-x64/gtkwave.zip", "$(builddir)/$(plat)/$(arch)/$(mode)/")
                end
                os.cd("$(builddir)/$(plat)/$(arch)/$(mode)/")
                if os.exec then
                    os.exec("tar -xf gtkwave.zip")
                else
                    os.run("tar -xf gtkwave.zip")
                end
                os.rm("gtkwave.zip")
            end
        elseif is_plat("windows") then
            local dir = "$(builddir)/$(plat)/$(arch)/$(mode)/gtkwave"
            if not os.isdir(dir) then
                os.cp("gtkwave/win-x64/gtkwave.zip", "$(builddir)/$(plat)/$(arch)/$(mode)/")
                os.cd("$(builddir)/$(plat)/$(arch)/$(mode)/")
                if os.exec then
                    os.exec("tar -xf gtkwave.zip")
                else
                    os.run("tar -xf gtkwave.zip")
                end
                os.rm("gtkwave.zip")
            end
        elseif is_plat("linux") then
            local dir = "$(builddir)/$(plat)/$(arch)/$(mode)/gtkwave"
            if not os.isdir(dir) then
                os.cp("gtkwave/linux-x64/gtkwave.tar.gz", "$(builddir)/$(plat)/$(arch)/$(mode)/")
                os.cd("$(builddir)/$(plat)/$(arch)/$(mode)/")
                if os.exec then
                    os.exec("tar -xf gtkwave.tar.gz")
                else
                    os.run("tar -xf gtkwave.tar.gz")
                end
                os.rm("gtkwave.tar.gz")
            end
        end
    end)