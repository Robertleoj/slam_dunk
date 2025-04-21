#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <slamd_window/window.hpp>

namespace slamdw {

Window::Window(
    std::string name
)
    : name(name),
      loaded_layout(false) {}

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

void Window::run(
    size_t height,
    size_t width
) {
    this->window = glutils::make_window("Slam Dunk", width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    if (fs::exists(this->layout_path())) {
        ImGui::LoadIniSettingsFromDisk(this->layout_path().string().c_str());
        this->loaded_layout = true;
    }

    while (!glfwWindowShouldClose(window)) {
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

Window::~Window() {}

}  // namespace slamdw