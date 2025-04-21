#pragma once

#include <memory>
#include <slamd_window/camera_2d.hpp>
#include <slamd_window/frame_buffer.hpp>
#include <slamd_window/frame_timer.hpp>
#include <slamd_window/tree/tree.hpp>
#include <slamd_window/view/view.hpp>

namespace slamdw {
class CanvasView : public View {
   public:
    std::shared_ptr<Tree> tree;

   private:
    FrameBuffer frame_buffer;
    Camera2D camera;
    FrameTimer frame_timer;
    bool manually_moved;

   public:
    CanvasView(std::shared_ptr<Tree> tree);
    void render_to_imgui() override;

   private:
    void render_to_frame_buffer();
    void set_default_pos();
    void fix_view_aspect();

    void handle_input();
    void handle_translation_input();
    void handle_mouse_input();
    glm::vec2 get_normalized_mouse_pos();
};

}  // namespace slamdw