#pragma once
#include <glbinding/gl/gl.h>
#include <optional>
#include <thread>

namespace slamd {

class FrameBuffer {
   private:
    struct GLData {
        uint frame_buffer_object_id;
        uint texture_id;
        uint render_buffer_object_id;
    };

    std::optional<GLData> gl_data;
    std::optional<std::thread::id> render_thread_id;

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
    void maybe_initialize();
};

}  // namespace slamd