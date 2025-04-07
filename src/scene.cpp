#include <imgui.h>
#include <spdlog/spdlog.h>
#include <slam_dunk/angle.hpp>
#include <slam_dunk/scene.hpp>

namespace sdunk {

Scene::Scene()
    : frame_buffer(500, 500),
      camera(45.0, 0.1f, 100000.0f) {}

void Scene::render_to_imgui() {
    arcball.rotate(Angle::deg(0.2), Angle::deg(0.05));

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

    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glEnable(gl::GL_DEPTH_TEST);

    gl::glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    auto view = this->arcball.view_matrix();
    auto projection =
        this->camera.get_projection_matrix(this->frame_buffer.aspect());

    this->tree.render(view, projection);

    this->frame_buffer.unbind();
}

}  // namespace sdunk