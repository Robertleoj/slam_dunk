
#include <imgui.h>
#include <slamd/canvas_view.hpp>

namespace slamd {

CanvasView::CanvasView(
    std::shared_ptr<Canvas> canvas
)
    : canvas(canvas),
      frame_buffer(500, 500) {}

void CanvasView::render_to_imgui() {
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

void CanvasView::render_to_frame_buffer() {
    this->frame_buffer.bind();
    gl::glViewport(
        0, 0, this->frame_buffer.width(), this->frame_buffer.height()
    );

    gl::glEnable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glEnable(gl::GL_DEPTH_TEST);

    gl::glClearColor(0.5686f, 0.0980f, 0.4196f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    // TODO: render calls here

    this->frame_buffer.unbind();
}

void CanvasView::handle_input() {
    // TODO: handle input
}

}  // namespace slamd