#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#include "game.h"

int main(int, char**) {
    glfwSetErrorCallback([](int error, const char* description) { spdlog::error("GLFW error {}: {}", error, description); });
    if (!glfwInit()) return 1;

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    auto window = glfwCreateWindow(1280, 720, "GLFW + OpenGL + Dear ImGui", nullptr, nullptr);
    if (!window) return 1;

    // Set window icon
    // {
    //     GLFWimage images[1];
    //     images[0].pixels = stbi_load("assets/icon.png", &images[0].width, &images[0].height, 0, 4);
    //     glfwSetWindowIcon(window, 1, images);
    //     stbi_image_free(images[0].pixels);
    // }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    if (gladLoadGL() == 0) {  // Initialize OpenGL loader
        spdlog::error("Failed to initialize OpenGL loader!");
        return 1;
    }

    {
        int major, minor, rev;
        glfwGetVersion(&major, &minor, &rev);
        spdlog::info("GLFW {}.{}.{}, OpenGL {}", major, minor, rev, glGetString(GL_VERSION));
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg
    // so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    io.Fonts->AddFontFromFileTTF("data/Roboto-Medium.ttf", 16.0f);

    Game game;
    glfwSetWindowUserPointer(window, &game);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        } else if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
            if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
                glfwRestoreWindow(window);
            } else {
                glfwMaximizeWindow(window);
            }
        } else {
            auto game = (Game*)glfwGetWindowUserPointer(window);
            game->key_callback(key, scancode, action, mods, ImGui::GetIO().WantCaptureKeyboard);
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        if (!ImGui::GetIO().WantCaptureMouse) {
            auto game = (Game*)glfwGetWindowUserPointer(window);
            game->mouse_pos_callback(xpos, ypos);
        }
    });

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        game.imgui_frame();

        ImGui::End();
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        game.gl_frame();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            auto ctx = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(ctx);
        }

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
