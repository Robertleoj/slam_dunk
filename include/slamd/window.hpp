#pragma once
#include <cstddef>
#include <memory>
#include <mutex>
#include <slamd/glfw.hpp>
#include <slamd/scene_view.hpp>
#include <thread>
#include <vector>

namespace slamd {
class Window {
   public:
    Window(size_t height, size_t width);
    ~Window();
    void wait_for_close();
    void add_scene(std::string name, std::shared_ptr<Scene> scene);

   private:
    void render_job(size_t height, size_t width);

   private:
    std::thread render_thread;
    GLFWwindow* window = nullptr;
    std::map<std::string, SceneView> scene_view_map;
    bool should_stop = false;
    std::mutex scene_view_map_mutex;
};

}  // namespace slamd