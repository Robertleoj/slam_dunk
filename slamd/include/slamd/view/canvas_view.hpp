#pragma once

#include <memory>
#include <slamd/frame_buffer.hpp>
#include <slamd/tree/tree.hpp>
#include <slamd/view/view.hpp>

namespace slamd {

class CanvasView : public View {
   public:
    std::shared_ptr<Canvas> canvas;

   private:
    FrameBuffer frame_buffer;

   public:
    CanvasView(std::shared_ptr<Canvas> canvas);
    void render_to_imgui() override;

   private:
    void render_to_frame_buffer();
    void handle_input();
};

}  // namespace slamd