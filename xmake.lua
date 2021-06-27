add_rules("mode.debug", "mode.release")

add_requires("fmt ^7.1.3", "glad ^0.1.34", "glfw ^3.3.4", "glm ^0.9.9", "spdlog ^1.8.5", "stb", "imgui-docking")

target("app")
    set_kind("binary")
    set_languages("cxx17")
    add_files("src/*.cpp")
    add_packages("fmt", "glad", "glfw", "glm", "spdlog", "stb", "imgui-docking")

task("format")
    set_category("plugin")
    set_menu {
        usage = "xmake format",
        description = "Format code inside src/ directory using clang-format",
        options = {}
    }

    on_run(function()
        for _, file in ipairs(table.join(os.files("$(projectdir)/src/**.cpp"), os.files("$(projectdir)/src/**.h"))) do
            print("Formatting " .. path.relative(file))
            os.execv("clang-format", {"-i", file})
        end
    end)

package("imgui-docking")
    add_urls("https://github.com/ocornut/imgui/archive/ac35b4bba2c30ad3df88ce8b86aaa8877099fc96.tar.gz")
    add_versions("1.83", "dfdbbb9f7d7e0171e5388ad692de199811562d6f9aaf0d8bc39f93696c6e06f1")

    if is_plat("windows", "mingw") then
        add_syslinks("Imm32")
    end

    on_install("macosx", "linux", "windows", "mingw", "android", "iphoneos", function (package)
        io.writefile("xmake.lua", [[
            add_requires("glad ^0.1.34", "glfw ^3.3.4")
            target("imgui-docking")
                set_kind("static")
                add_includedirs(".")
                add_files("*.cpp", "backends/imgui_impl_glfw.cpp", "backends/imgui_impl_opengl3.cpp")
                add_headerfiles("imgui.h", "imconfig.h", "backends/imgui_impl_glfw.h", "backends/imgui_impl_opengl3.h")
                add_packages("glad", "glfw")
        ]])
        import("package.tools.xmake").install(package)
    end)
package_end()
