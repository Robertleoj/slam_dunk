#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <format>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

glm::mat4 random_transform() {
    // Seed once (if not already seeded somewhere else)
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(0)));
        seeded = true;
    }

    glm::vec3 position(
        (std::rand() % 200 - 100) / 10.0f,  // -10.0 to 10.0
        (std::rand() % 200 - 100) / 10.0f,
        (std::rand() % 200 - 100) / 10.0f
    );

    float angle = static_cast<float>(std::rand() % 360);  // 0 to 359 degrees

    glm::vec3 axis(
        (std::rand() % 200 - 100) / 100.0f,
        (std::rand() % 200 - 100) / 100.0f,
        (std::rand() % 200 - 100) / 100.0f
    );
    axis = glm::normalize(axis);  // make sure it's a unit vector

    // glm::vec3 scale(
    //     (std::rand() % 200) / 100.0f + 0.1f,  // 0.1 to 2.0
    //     (std::rand() % 200) / 100.0f + 0.1f,
    //     (std::rand() % 200) / 100.0f + 0.1f
    // );

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    // glm::scale(glm::mat4(1.0f), scale);

    return transform;
}

int main() {
    spdlog::set_level(spdlog::level::debug);

    const uint window_height = 1000, window_width = 1000;
    auto window =
        sdunk::glutils::make_window("Slam Dunk", window_width, window_height);

    sdunk::Scene scene{};

    for (int i = 0; i < 100; i++) {
        std::string box_path = std::format("/box{}", i);

        scene.tree.set_object(box_path, std::make_shared<sdunk::Box>());
        scene.tree.set_transform(box_path, random_transform());
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

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