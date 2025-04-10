#pragma once
#include <cstddef>
#include <filesystem>
#include <memory>
#include <mutex>
#include <slamd/glfw.hpp>
#include <slamd/view/canvas_view.hpp>
#include <slamd/view/scene_view.hpp>
#include <thread>
#include <vector>

namespace slamd {

namespace fs = std::filesystem;

class Window {
   public:
    Window(std::string name, size_t height, size_t width);
    ~Window();
    void wait_for_close();
    void add_scene(std::string name, std::shared_ptr<Scene> scene);
    void add_canvas(std::string name, std::shared_ptr<Canvas> canvas);

   private:
    void render_job(size_t height, size_t width);
    fs::path layout_path();

   private:
    std::string name;
    std::thread render_thread;
    GLFWwindow* window = nullptr;

    bool should_stop = false;

    std::mutex view_map_mutex;
    std::map<std::string, std::shared_ptr<View>> view_map;

    bool loaded_layout;
};

}  // namespace slamd