#pragma once
#include <cstddef>
#include <memory>
#include <mutex>
#include <slamd/glfw.hpp>
#include <slamd/view/canvas_view.hpp>
#include <slamd/view/scene_view.hpp>
#include <thread>
#include <vector>

namespace slamd {
class Window {
   public:
    Window(size_t height, size_t width);
    ~Window();
    void wait_for_close();
    void add_scene(std::string name, std::shared_ptr<Scene> scene);
    void add_canvas(std::string name, std::shared_ptr<Canvas> canvas);

   private:
    void render_job(size_t height, size_t width);

   private:
    std::thread render_thread;
    GLFWwindow* window = nullptr;

    bool should_stop = false;

    std::mutex view_map_mutex;
    std::map<std::string, std::shared_ptr<View>> view_map;
};

}  // namespace slamd