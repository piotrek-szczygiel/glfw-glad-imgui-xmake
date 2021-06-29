#pragma once

class Game {
   public:
    void key_callback(int key, int scancode, int action, int mods, bool inside_imgui);
    void mouse_pos_callback(double x, double y);

    void update();

    void gl_frame();
    void imgui_frame();
};
