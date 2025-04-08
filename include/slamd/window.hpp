#pragma once
#include <cstddef>
#include <slamd/glfw.hpp>
#include <thread>

namespace slamd {
class Window {
   public:
    Window(size_t height, size_t width);

   private:
    void render_job(size_t height, size_t width);

   private:
    std::thread render_thread;
    GLFWwindow* window = nullptr;
};

}  // namespace slamd