#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <slam_dunk/glfw.hpp>
#include <slam_dunk/slamvas.hpp>

namespace sdunk {

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
) {
    spdlog::debug("Window resized");
    gl::glViewport(0, 0, width, height);
}

void process_input(
    GLFWwindow* window
) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

Slamvas::Slamvas() {
    render_thread = std::thread(&Slamvas::render_job, this);
}

void Slamvas::render_job() {
    const uint window_height = 1000, window_width = 1000;
    auto window =
        glutils::make_window("Slam Dunk", window_width, window_height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!this->should_stop) {
        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Slamvas::~Slamvas() {
    this->should_stop = true;
    if (this->render_thread.joinable()) {
        this->render_thread.join();
    }
}

}  // namespace sdunk