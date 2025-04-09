#pragma once

#include <memory>
#include <slamd/frame_buffer.hpp>
#include <slamd/tree/tree.hpp>

namespace slamd {

class CanvasView {
   public:
    std::shared_ptr<Canvas> canvas;

   private:
    FrameBuffer frame_buffer;

   public:
    CanvasView(std::shared_ptr<Canvas> canvas);
    void render_to_imgui();

   private:
    void render_to_frame_buffer();
    void handle_input();
};

}  // namespace slamd