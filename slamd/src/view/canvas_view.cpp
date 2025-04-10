
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <slamd/view/canvas_view.hpp>

namespace slamd {

CanvasView::CanvasView(
    std::shared_ptr<Canvas> canvas
)
    : canvas(canvas),
      frame_buffer(500, 500),
      camera(_geom::AABB2D({0.0, 1.0}, {-1.0, 0.0})),
      manually_moved(false) {}

void CanvasView::render_to_imgui() {
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    int width = static_cast<int>(availSize.x);
    int height = static_cast<int>(availSize.y);
    this->frame_buffer.rescale(width, height);

    if (!this->manually_moved) {
        this->set_default_pos();
    } else {
        this->fix_view_aspect();
    }

    this->render_to_frame_buffer();

    ImGui::Image(
        (ImTextureID)this->frame_buffer.frame_texture(),
        ImGui::GetContentRegionAvail(),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    this->frame_timer.log_frame();
    this->handle_input();
}

void CanvasView::render_to_frame_buffer() {
    this->frame_buffer.bind();
    gl::glViewport(
        0,
        0,
        this->frame_buffer.width(),
        this->frame_buffer.height()
    );

    gl::glEnable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glEnable(gl::GL_DEPTH_TEST);

    gl::glClearColor(0.5686f, 0.0980f, 0.4196f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    glm::mat4 projection_matrix = this->camera.get_projection_matrix();

    // TODO: render calls here
    this->canvas->render(glm::mat4(1.0), projection_matrix);

    this->frame_buffer.unbind();
}

void CanvasView::handle_input() {
    // TODO: handle input
    if (ImGui::IsWindowFocused()) {
        this->handle_translation_input();

        if (ImGui::IsKeyPressed(ImGuiKey_Period, false)) {
            this->set_default_pos();
            this->manually_moved = false;
        }
    }
}
void CanvasView::handle_translation_input() {
    glm::vec2 translation(0.0f, 0.0f);
    float movement_amount = this->frame_timer.timedelta();

    glm::vec2 right(movement_amount, 0.0f);
    glm::vec2 up(0.0, movement_amount);

    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        translation += right;
    }

    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        translation -= right;
    }

    if (ImGui::IsKeyDown(ImGuiKey_E) || ImGui::IsKeyDown(ImGuiKey_W)) {
        translation += up;
    }

    if (ImGui::IsKeyDown(ImGuiKey_Q) || ImGui::IsKeyDown(ImGuiKey_S)) {
        translation -= up;
    }

    if (glm::length(translation) > 1e-6f) {
        this->manually_moved = true;
        this->camera.translate_relative(translation);
    }
}

void CanvasView::set_default_pos() {
    auto maybe_bounds = this->canvas->bounds();

    _geom::AABB bounds = maybe_bounds.has_value()
                             ? maybe_bounds.value()
                             : _geom::AABB(
                                   glm::vec3(0.0f, -1.0f, 0.0f),
                                   glm::vec3(1.0f, 0.0f, 0.0f)
                               );

    float window_aspect = this->frame_buffer.aspect();

    this->camera.set_viewport(
        _geom::AABB2D::center_cover(
            _geom::AABB2D::from_3d(bounds),
            window_aspect
        )
    );
}

void CanvasView::fix_view_aspect() {
    float window_aspect = this->frame_buffer.aspect();
    this->camera.set_viewport(
        _geom::AABB2D::center_cover(this->camera.viewport, window_aspect)
    );
}

}  // namespace slamd