#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <slamd/render_thread_job_queue.hpp>
#include <slamd/window.hpp>

namespace slamd {

Window::Window(
    std::string name,
    size_t height,
    size_t width
)
    : name(name),
      loaded_layout(false) {
    this->render_thread = std::thread(&Window::render_job, this, height, width);
}

fs::path Window::layout_path() {
    return fs::current_path() / std::format(".{}.ini", this->name);
}

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
) {
    gl::glViewport(0, 0, width, height);
}

void Window::add_scene(
    std::string name,
    std::shared_ptr<Scene> scene
) {
    std::scoped_lock l(this->view_map_mutex);

    this->view_map.emplace(name, std::make_shared<SceneView>(scene));
}

void Window::add_canvas(
    std::string name,
    std::shared_ptr<Canvas> canvas
) {
    std::scoped_lock l(this->view_map_mutex);

    this->view_map.emplace(name, std::make_shared<CanvasView>(canvas));
}

void Window::render_job(
    size_t height,
    size_t width
) {
    RenderQueueManager::ensure_current_thread_queue();

    auto render_queue = RenderQueueManager::get_current_thread_queue().value();

    this->window = slamd::glutils::make_window("Slam Dunk", width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    if (fs::exists(this->layout_path())) {
        ImGui::LoadIniSettingsFromDisk(this->layout_path().string().c_str());
        this->loaded_layout = true;
    }

    while (!glfwWindowShouldClose(window) && !should_stop) {
        render_queue->execute_all();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiID main_dockspace_id;
        main_dockspace_id = ImGui::DockSpaceOverViewport();

        {
            std::scoped_lock l(this->view_map_mutex);

            for (auto& [scene_name, scene] : this->view_map) {
                if (!this->loaded_layout) {
                    ImGui::SetNextWindowDockID(
                        main_dockspace_id,
                        ImGuiCond_Once
                    );
                }
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

                ImGui::Begin(
                    scene_name.c_str(),
                    nullptr,
                    ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse
                );
                scene->render_to_imgui();
                ImGui::End();
                ImGui::PopStyleVar();
            }
        }

        ImGui::Render();

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui::SaveIniSettingsToDisk(this->layout_path().string().c_str());

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