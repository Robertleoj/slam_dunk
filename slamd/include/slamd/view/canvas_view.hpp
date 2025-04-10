#pragma once

#include <memory>
#include <slamd/camera_2d.hpp>
#include <slamd/frame_buffer.hpp>
#include <slamd/frame_timer.hpp>
#include <slamd/tree/tree.hpp>
#include <slamd/view/view.hpp>

namespace slamd {

class CanvasView : public View {
   public:
    std::shared_ptr<Canvas> canvas;

   private:
    FrameBuffer frame_buffer;
    Camera2D camera;
    FrameTimer frame_timer;
    bool manually_moved;

   public:
    CanvasView(std::shared_ptr<Canvas> canvas);
    void render_to_imgui() override;

   private:
    void render_to_frame_buffer();
    void set_default_pos();
    void fix_view_aspect();

    void handle_input();
    void handle_translation_input();
    void handle_mouse_input();
};

}  // namespace slamd