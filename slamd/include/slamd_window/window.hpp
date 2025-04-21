#pragma once
#include <cstddef>
#include <filesystem>
#include <memory>
#include <mutex>
#include <slamd_window/glfw.hpp>
#include <slamd_window/state_manager.hpp>
#include <slamd_window/view/canvas_view.hpp>
#include <slamd_window/view/scene_view.hpp>
#include <stop_token>
#include <thread>
#include <vector>

namespace slamdw {

namespace fs = std::filesystem;

class Window {
   public:
    Window(size_t height = 1000, size_t width = 1000);
    ~Window();

    void run();

    StateManager state_manager;

   private:
    // std::string name;
    GLFWwindow* window = nullptr;

    // std::mutex view_map_mutex;
    // std::map<std::string, std::shared_ptr<View>> view_map;
    // bool loaded_layout;
};

}  // namespace slamdw