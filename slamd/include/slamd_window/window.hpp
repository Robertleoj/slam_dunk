#pragma once
#include <slamd_window/glfw.hpp>
#include <slamd_window/state_manager.hpp>
#include <slamd_window/view/canvas_view.hpp>
#include <slamd_window/view/scene_view.hpp>
namespace slamd {


class Window {
   public:
    Window();
    ~Window();

    void run();

    StateManager state_manager;

   private:
    GLFWwindow* window = nullptr;
};

}  // namespace slamd