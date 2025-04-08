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

void seed_random() {
    std::srand(static_cast<unsigned int>(std::time(0)));
}

float rand_float() {
    return static_cast<float>(std::rand()) / RAND_MAX;
}

float rand_float(
    float min,
    float max
) {
    return rand_float() * (max - min) + min;
}

glm::vec3 random_vector(
    float min,
    float max
) {
    return glm::vec3(
        rand_float(min, max), rand_float(min, max), rand_float(min, max)
    );
}

glm::mat4 random_transform(
    bool scale = false
) {
    glm::vec3 position = random_vector(-10.0, 10.0);

    float angle = rand_float(0.0f, 359.0f);

    glm::vec3 axis = random_vector(0.0, 10.0);

    axis = glm::normalize(axis);  // make sure it's a unit vector

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

    if (scale) {
        glm::vec3 random_scale = random_vector(0.1f, 2.0f);

        transform = transform * glm::scale(glm::mat4(1.0f), random_scale);
    }

    return transform;
}

std::shared_ptr<slamd::Sphere> random_sphere() {
    float radius = rand_float(0.1, 1.0);

    glm::vec3 color = random_vector(0.0f, 1.0f);
    return std::make_shared<slamd::Sphere>(radius, color);
}

std::shared_ptr<slamd::PolyLine> random_poly_line(
    int pointCount = 10
) {
    float thickness = rand_float(0.01, 0.5);
    std::vector<glm::vec3> points;

    glm::vec3 pos = random_vector(-10.0f, 10.0f);  // Start at origin
    glm::vec3 dir =
        glm::normalize(random_vector(-1.0, 1.0f));  // Initial direction

    for (int i = 0; i < pointCount; ++i) {
        // Add some randomness to the direction, keep it smooth
        dir += random_vector(-0.2, 0.2);
        dir = glm::normalize(dir);
        pos += dir * 0.5f;  // Step size

        points.push_back(pos);
    }

    glm::vec3 color = random_vector(0.0, 1.0);

    return std::make_shared<slamd::PolyLine>(points, thickness, color);
}

std::shared_ptr<slamd::Arrows> random_arrows(
    int arrowCount = 10
) {
    float thickness = rand_float(0.01f, 0.5f);

    std::vector<glm::vec3> starts;
    std::vector<glm::vec3> ends;
    std::vector<glm::vec3> colors;

    for (int i = 0; i < arrowCount; ++i) {
        glm::vec3 dir = glm::normalize(random_vector(-1.0f, 1.0f));
        float length = rand_float(0.5f, 3.0f);
        glm::vec3 origin = random_vector(-10.0, 10.0);
        glm::vec3 end = origin + dir * length;

        starts.push_back(origin);
        ends.push_back(end);
        colors.push_back(random_vector(0.0f, 1.0f));
    }

    return std::make_shared<slamd::Arrows>(starts, ends, colors, thickness);
}

int main() {
    seed_random();
    spdlog::set_level(spdlog::level::debug);

    const uint window_height = 1000, window_width = 1000;
    auto window =
        slamd::glutils::make_window("Slam Dunk", window_width, window_height);

    slamd::Scene scene{};

    scene.tree.set_object("/origin_triad", std::make_shared<slamd::Triad>());

    for (int i = 0; i < 100; i++) {
        std::string box_path = std::format("/box{}", i);

        scene.tree.set_object(box_path, std::make_shared<slamd::Box>());
        scene.tree.set_transform(box_path, random_transform(true));
    }

    for (int i = 0; i < 50; i++) {
        std::string sphere_path = std::format("/sphere{}", i);

        scene.tree.set_object(sphere_path, random_sphere());
        scene.tree.set_transform(sphere_path, random_transform(false));
    }

    for (int i = 0; i < 20; i++) {
        std::string poly_line_path = std::format("/poly_line{}", i);

        scene.tree.set_object(poly_line_path, random_poly_line());
    }

    scene.tree.set_object("/arrows", random_arrows(10));

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