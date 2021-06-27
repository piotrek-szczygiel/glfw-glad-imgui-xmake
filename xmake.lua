add_rules("mode.debug", "mode.release")

package("imgui-docking")
    set_homepage("https://github.com/ocornut/imgui/tree/docking")
    set_description("Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies")
    add_urls("https://github.com/ocornut/imgui/archive/ac35b4bba2c30ad3df88ce8b86aaa8877099fc96.tar.gz")
    add_versions("1.83", "dfdbbb9f7d7e0171e5388ad692de199811562d6f9aaf0d8bc39f93696c6e06f1")

    if is_plat("windows", "mingw") then
        add_syslinks("Imm32")
    end

    on_install("macosx", "linux", "windows", "mingw", "android", "iphoneos", function (package)
        local xmake_lua = [[
            target("imgui")
                set_kind("static")
                add_includedirs(".")
                add_files("*.cpp", "backends/imgui_impl_glfw.cpp", "backends/imgui_impl_opengl3.cpp")
                add_headerfiles("imgui.h", "imconfig.h", "backends/imgui_impl_glfw.h", "backends/imgui_impl_opengl3.h")
        ]]

        io.writefile("xmake.lua", xmake_lua)
        import("package.tools.xmake").install(package)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            void test() {
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO();
                ImGui::NewFrame();
                ImGui::Text("Hello, world!");
                ImGui::ShowDemoWindow(NULL);
                ImGui::Render();
                ImGui::DestroyContext();
            }
        ]]}, {configs = {languages = "c++11"}, includes = {"imgui.h"}}))
    end)
package_end()

add_requires("glad", "glfw", "imgui-docking")

target("test")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glad", "glfw", "imgui-docking")
