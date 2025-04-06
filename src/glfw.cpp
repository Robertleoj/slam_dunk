#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <spdlog/spdlog.h>
#include <slam_dunk/glfw.hpp>
#include <stdexcept>

namespace sdunk {
namespace glutils {

GLFWwindow* make_window(
    std::string window_name,
    std::size_t width,
    std::size_t height
) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    spdlog::info("GLFW window created");

    glfwMakeContextCurrent(window);

    glbinding::initialize(glfwGetProcAddress);

    gl::glViewport(0, 0, width, height);

    return window;
}
}  // namespace glutils
}  // namespace sdunk