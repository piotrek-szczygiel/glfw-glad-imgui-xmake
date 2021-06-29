#include "game.h"

#include <glad/glad.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

void Game::key_callback(int key, int scancode, int action, int mods, bool inside_imgui) {
    spdlog::debug("Key {}, Scancode {}, Action {}, Mods {}, ImGUI {}", key, scancode, action, mods, inside_imgui);
}

void Game::mouse_pos_callback(double x, double y) {}

void Game::update() {}

void Game::gl_frame() {
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::imgui_frame() {
    static bool log_debug = false;

    ImGui::Begin("Application");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (ImGui::Checkbox("Log debug", &log_debug)) {
        spdlog::set_level(log_debug ? spdlog::level::debug : spdlog::level::info);
    }
}
