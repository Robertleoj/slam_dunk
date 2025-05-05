#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <spdlog/spdlog.h>
#include <slamd_common/assert.hpp>
#include <slamd_window/frame_buffer.hpp>

namespace slamd {

void FrameBuffer::initialize() {
    gl::glGenFramebuffers(1, &this->frame_buffer_object_id);
    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, this->frame_buffer_object_id);

    gl::glGenTextures(1, &this->texture_id);
    gl::glBindTexture(gl::GL_TEXTURE_2D, this->texture_id);
    gl::glTexImage2D(
        gl::GL_TEXTURE_2D,
        0,
        gl::GL_RGB,
        this->current_width,
        this->current_height,
        0,
        gl::GL_RGB,
        gl::GL_UNSIGNED_BYTE,
        nullptr
    );
    gl::glTexParameteri(
        gl::GL_TEXTURE_2D,
        gl::GL_TEXTURE_MIN_FILTER,
        gl::GL_LINEAR
    );
    gl::glTexParameteri(
        gl::GL_TEXTURE_2D,
        gl::GL_TEXTURE_MAG_FILTER,
        gl::GL_LINEAR
    );

    gl::glFramebufferTexture2D(
        gl::GL_FRAMEBUFFER,
        gl::GL_COLOR_ATTACHMENT0,
        gl::GL_TEXTURE_2D,
        this->texture_id,
        0
    );

    gl::glGenRenderbuffers(1, &this->render_buffer_object_id);
    gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, this->render_buffer_object_id);
    gl::glRenderbufferStorage(
        gl::GL_RENDERBUFFER,
        gl::GL_DEPTH24_STENCIL8,
        this->current_width,
        this->current_height
    );

    gl::glFramebufferRenderbuffer(
        gl::GL_FRAMEBUFFER,
        gl::GL_DEPTH_STENCIL_ATTACHMENT,
        gl::GL_RENDERBUFFER,
        this->render_buffer_object_id
    );

    if (gl::glCheckFramebufferStatus(gl::GL_FRAMEBUFFER) !=
        gl::GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer not complete");
    }

    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
    gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
    gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, 0);
}

FrameBuffer::FrameBuffer(
    size_t width,
    size_t height
)
    : current_height(height),
      current_width(width) {
    this->initialize();
}

FrameBuffer::~FrameBuffer() {
    gl::glDeleteFramebuffers(1, &this->frame_buffer_object_id);
    gl::glDeleteTextures(1, &this->texture_id);
    gl::glDeleteRenderbuffers(1, &this->render_buffer_object_id);
}

uint FrameBuffer::frame_texture() {
    return this->texture_id;
}

void FrameBuffer::rescale(
    size_t width,
    size_t height
) {
    if (this->current_height == height && this->current_width == width) {
        return;
    }

    gl::glViewport(0, 0, width, height);

    gl::glBindTexture(gl::GL_TEXTURE_2D, this->texture_id);
    gl::glTexImage2D(
        gl::GL_TEXTURE_2D,
        0,
        gl::GL_RGB,
        width,
        height,
        0,
        gl::GL_RGB,
        gl::GL_UNSIGNED_BYTE,
        nullptr
    );
    gl::glTexParameteri(
        gl::GL_TEXTURE_2D,
        gl::GL_TEXTURE_MIN_FILTER,
        gl::GL_LINEAR
    );

    gl::glTexParameteri(
        gl::GL_TEXTURE_2D,
        gl::GL_TEXTURE_MAG_FILTER,
        gl::GL_LINEAR
    );
    gl::glFramebufferTexture2D(
        gl::GL_FRAMEBUFFER,
        gl::GL_COLOR_ATTACHMENT0,
        gl::GL_TEXTURE_2D,
        this->texture_id,
        0
    );

    gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, this->render_buffer_object_id);
    gl::glRenderbufferStorage(
        gl::GL_RENDERBUFFER,
        gl::GL_DEPTH24_STENCIL8,
        width,
        height
    );
    gl::glFramebufferRenderbuffer(
        gl::GL_FRAMEBUFFER,
        gl::GL_DEPTH_STENCIL_ATTACHMENT,
        gl::GL_RENDERBUFFER,
        this->render_buffer_object_id
    );

    this->current_height = height;
    this->current_width = width;
}

void FrameBuffer::bind() {
    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, this->frame_buffer_object_id);
    gl::glViewport(0, 0, this->width(), this->height());
}

void FrameBuffer::unbind() {
    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
}

double FrameBuffer::aspect() const {
    return static_cast<double>(this->current_width) /
           static_cast<double>(this->current_height);
}

size_t FrameBuffer::width() const {
    return this->current_width;
}

size_t FrameBuffer::height() const {
    return this->current_height;
}

}  // namespace slamd