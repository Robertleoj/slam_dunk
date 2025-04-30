#pragma once
#include <glbinding/gl/gl.h>
#include <optional>
#include <thread>

namespace slamd {

class FrameBuffer {
   private:
    uint frame_buffer_object_id = 0;
    uint texture_id = 0;
    uint render_buffer_object_id = 0;

    size_t current_height;
    size_t current_width;

   public:
    FrameBuffer(size_t width, size_t height);
    ~FrameBuffer();

    uint frame_texture();
    void rescale(size_t width, size_t height);
    void bind();
    void unbind();
    double aspect() const;
    size_t width() const;
    size_t height() const;

   private:
    void initialize();
};

}  // namespace slamd