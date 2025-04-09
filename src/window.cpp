#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <slamd/window.hpp>

namespace slamd {

Window::Window(
    size_t height,
    size_t width
) {
    this->render_thread = std::thread(&Window::render_job, this, height, width);
}

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
) {
    spdlog::debug("Window resized");
    gl::glViewport(0, 0, width, height);
}

void Window::add_scene(
    std::string name,
    std::shared_ptr<Scene> scene
) {
    std::scoped_lock l(this->scene_view_map_mutex);

    this->scene_view_map.emplace(name, scene);
}

void Window::render_job(
    size_t height,
    size_t width
) {
    this->window = slamd::glutils::make_window("Slam Dunk", width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window) && !should_stop) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiID main_dockspace_id = ImGui::DockSpaceOverViewport();

        {
            std::scoped_lock l(this->scene_view_map_mutex);

            for (auto& [scene_name, scene] : this->scene_view_map) {
                ImGui::SetNextWindowDockID(main_dockspace_id, ImGuiCond_Once);
                ImGui::Begin(scene_name.c_str());
                scene.render_to_imgui();
                ImGui::End();
            }
        }

        ImGui::Render();

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Window::wait_for_close() {
    if (this->render_thread.joinable()) {
        this->render_thread.join();
    }
}

Window::~Window() {
    should_stop = true;
    if (this->render_thread.joinable()) {
        this->render_thread.join();
    }
}

}  // namespace slamd