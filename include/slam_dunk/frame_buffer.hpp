#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

namespace sdunk {

class FrameBuffer {
   public:
    FrameBuffer(size_t width, size_t height);
    ~FrameBuffer();

    gl::GLuint frame_texture();
    void rescale(size_t width, size_t height);
    void bind() const;
    void unbind() const;
    double aspect() const;
    size_t width() const;
    size_t height() const;

   private:
    void render_to_frame_buffer();
    gl::GLuint frame_buffer_object_id;
    gl::GLuint texture_id;
    gl::GLuint render_buffer_object_id;
    size_t current_height;
    size_t current_width;
};

}  // namespace sdunk