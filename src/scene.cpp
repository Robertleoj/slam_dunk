#include <imgui.h>
#include <spdlog/spdlog.h>
#include <slam_dunk/scene.hpp>

namespace sdunk {

Scene::Scene()
    : frame_buffer(500, 500) {}

void Scene::render_to_imgui() {
    spdlog::debug("Rendering to imgui");
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    int width = static_cast<int>(availSize.x);
    int height = static_cast<int>(availSize.y);
    this->frame_buffer.rescale(width, height);

    this->render_to_frame_buffer();

    ImGui::Image(
        (ImTextureID)this->frame_buffer.frame_texture(),
        ImGui::GetContentRegionAvail(),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );
}

void Scene::render_to_frame_buffer() {
    this->frame_buffer.bind();

    spdlog::debug("Rendering triangle");

    gl::glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    gl::glBegin(gl::GL_TRIANGLES);
    gl::glColor3f(1.0, 0.0, 0.0);  // Red
    gl::glVertex2f(-0.5f, -0.5f);

    gl::glColor3f(0.0, 1.0, 0.0);  // Green
    gl::glVertex2f(0.5f, -0.5f);

    gl::glColor3f(0.0, 0.0, 1.0);  // Blue
    gl::glVertex2f(0.0f, 0.5f);
    gl::glEnd();

    gl::glFlush();

    this->frame_buffer.unbind();
}

}  // namespace sdunk