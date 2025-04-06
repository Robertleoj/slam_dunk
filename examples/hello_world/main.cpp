#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <slam_dunk/glfw.hpp>
#include <slam_dunk/slam_dunk.hpp>
#include <thread>

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
) {
    spdlog::debug("Window resized");
    gl::glViewport(0, 0, width, height);
}

int main() {
    spdlog::set_level(spdlog::level::debug);

    const uint window_height = 1000, window_width = 1000;
    auto window =
        sdunk::glutils::make_window("Slam Dunk", window_width, window_height);

    sdunk::Scene scene{};

    // scene.tree.set_object(
    //     sdunk::TreePath("/box"), std::make_shared<sdunk::Box>()
    // );
    scene.tree.set_object("/box", std::make_shared<sdunk::Box>());

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiID main_dockspace_id = ImGui::DockSpaceOverViewport();

        ImGui::SetNextWindowDockID(main_dockspace_id, ImGuiCond_Once);
        ImGui::Begin("Scene");
        scene.render_to_imgui();
        ImGui::End();

        ImGui::Render();

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}