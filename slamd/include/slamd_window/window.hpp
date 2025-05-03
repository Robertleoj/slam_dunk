#pragma once
#include <filesystem>
#include <slamd_window/glfw.hpp>
#include <slamd_window/state_manager.hpp>
#include <slamd_window/view/canvas_view.hpp>
#include <slamd_window/view/scene_view.hpp>
namespace slamd {

namespace fs = std::filesystem;

class Window {
   public:
    Window(std::string name);
    ~Window();

    void run();

    StateManager state_manager;
    fs::path layout_path();

   private:
    std::string name;
    GLFWwindow* window = nullptr;

    bool loaded_layout;
};

}  // namespace slamd