#include <imgui.h>
#include <spdlog/spdlog.h>
#include <numbers>
#include <slam_dunk/angle.hpp>
#include <slam_dunk/scene.hpp>

namespace sdunk {

Scene::Scene()
    : frame_buffer(500, 500),
      camera(45.0, 0.1f, 100000.0f) {}

void Scene::render_to_imgui() {
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
    this->handle_input();
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

void Scene::handle_input() {
    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::IsWindowFocused()) {
        // Mouse controls - only if the window is hovered
        if (ImGui::IsItemHovered()) {
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                // handle dragging
                auto mouse_drag_delta =
                    ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

                auto mouse_drag_delta_x = static_cast<int>(mouse_drag_delta.x);
                auto mouse_drag_delta_y = static_cast<int>(mouse_drag_delta.y);

                ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
                spdlog::debug(
                    "Mouse dragged: x={} y={}",
                    mouse_drag_delta_x,
                    mouse_drag_delta_y
                );

                auto min_window_dim = std::min(
                    this->frame_buffer.width(), this->frame_buffer.height()
                );

                // pi per window size
                auto scale_factor =
                    std::numbers::pi / static_cast<float>(min_window_dim);

                auto x_angle_diff =
                    static_cast<float>(mouse_drag_delta_x) * scale_factor;
                auto y_angle_diff =
                    static_cast<float>(mouse_drag_delta_y) * scale_factor;

                this->arcball.rotate(
                    -Angle::rad(x_angle_diff), -Angle::rad(y_angle_diff)
                );
            }
        }

        glm::vec3 translation(0.0, 0.0, 0.0);
        float movement_amount = 0.1;

        glm::vec3 forwards(0.0, 0.0, -movement_amount);
        glm::vec3 right(movement_amount, 0.0, 0.0);
        glm::vec3 up(0.0, movement_amount, 0.0);

        if (ImGui::IsKeyDown(ImGuiKey_W)) {
            // move forward (negative z in camera space)
            translation += forwards;
        }

        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            translation -= forwards;
        }

        if (ImGui::IsKeyDown(ImGuiKey_D)) {
            translation += right;
        }

        if (ImGui::IsKeyDown(ImGuiKey_A)) {
            translation -= right;
        }

        if (ImGui::IsKeyDown(ImGuiKey_E)) {
            translation += up;
        }

        if (ImGui::IsKeyDown(ImGuiKey_Q)) {
            translation -= up;
        }

        this->arcball.translate(translation);
    }
    // arcball.rotate(Angle::deg(0.2), Angle::deg(0.05));
}

}  // namespace sdunk