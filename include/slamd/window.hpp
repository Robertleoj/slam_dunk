#pragma once
#include <cstddef>
#include <memory>
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

    ~Window();

   private:
    std::thread render_thread;
    GLFWwindow* window = nullptr;
    std::map<std::string, std::shared_ptr<Scene>> scenes;
    bool should_stop = false;
};

}  // namespace slamd