
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <slamd/view/canvas_view.hpp>

namespace slamd {

CanvasView::CanvasView(
    std::shared_ptr<Canvas> canvas
)
    : canvas(canvas),
      frame_buffer(500, 500),
      camera({0.0, 1.0}, {-1.0, 0.0}),
      manually_moved(false) {}

void CanvasView::render_to_imgui() {
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    int width = static_cast<int>(availSize.x);
    int height = static_cast<int>(availSize.y);
    this->frame_buffer.rescale(width, height);

    if (!this->manually_moved) {
        this->set_default_pos();
    }

    this->render_to_frame_buffer();

    ImGui::Image(
        (ImTextureID)this->frame_buffer.frame_texture(),
        ImGui::GetContentRegionAvail(),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

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

    float bounds_width = bounds.max.x - bounds.min.x;
    float bounds_height = bounds.max.y - bounds.min.y;
    float bounds_aspect = bounds_width / bounds_height;

    glm::vec2 center = 0.5f * (bounds.min + bounds.max);

    float half_width, half_height;

    if (window_aspect > bounds_aspect) {
        // Window is wider than content – pad width
        half_height = 0.5f * bounds_height;
        half_width = half_height * window_aspect;
    } else {
        // Window is taller than content – pad height
        half_width = 0.5f * bounds_width;
        half_height = half_width / window_aspect;
    }

    glm::vec2 min_view = center - glm::vec2(half_width, half_height);
    glm::vec2 max_view = center + glm::vec2(half_width, half_height);

    this->camera.set_bounds({min_view.x, max_view.x}, {min_view.y, max_view.y});
}

}  // namespace slamd