
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

    spdlog::debug("Manually moved");
    if (!this->manually_moved) {
        spdlog::debug("Setting default pos");
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

    if (!maybe_bounds.has_value()) {
        this->camera.set_bounds({0.0, 1.0}, {-1.0, 0.0});
        return;
    }

    auto bounds = maybe_bounds.value();

    spdlog::debug(
        "Setting bounds to x({}-{}) y({}-{})",
        bounds.min.x,
        bounds.max.x,
        bounds.min.y,
        bounds.max.y
    );

    this->camera.set_bounds(
        {bounds.min.x, bounds.max.x},
        {bounds.min.y, bounds.max.y}
    );
}

}  // namespace slamd