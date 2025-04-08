#pragma once
#include <cstddef>
#include <memory>
#include <mutex>
#include <slamd/glfw.hpp>
#include <slamd/scene.hpp>
#include <thread>
#include <vector>

namespace slamd {
class Window {
   public:
    Window(size_t height, size_t width);

   private:
    void render_job(size_t height, size_t width);

    void add_scene(std::string name, std::shared_ptr<Scene> scene);

    ~Window();

   private:
    std::thread render_thread;
    GLFWwindow* window = nullptr;
    std::map<std::string, std::shared_ptr<Scene>> scene_map;
    bool should_stop = false;
    std::mutex scene_map_mutex;
};

}  // namespace slamd