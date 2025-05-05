#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <slamd_window/frame_buffer.hpp>
#include <stdexcept>

using namespace gl;

namespace slamd {

void FrameBuffer::initialize() {
    // === MSAA FBO ===
    glGenFramebuffers(1, &msaa_framebuffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, msaa_framebuffer_id);

    glGenTextures(1, &msaa_color_buffer_id);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaa_color_buffer_id);
    glTexImage2DMultisample(
        GL_TEXTURE_2D_MULTISAMPLE,
        samples,
        GL_RGB8,
        current_width,
        current_height,
        GL_TRUE
    );
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D_MULTISAMPLE,
        msaa_color_buffer_id,
        0
    );

    glGenRenderbuffers(1, &msaa_depth_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, msaa_depth_buffer_id);
    glRenderbufferStorageMultisample(
        GL_RENDERBUFFER,
        samples,
        GL_DEPTH24_STENCIL8,
        current_width,
        current_height
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        msaa_depth_buffer_id
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("MSAA framebuffer not complete");
    }

    // === Resolve FBO ===
    glGenFramebuffers(1, &frame_buffer_object_id);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_object_id);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        current_width,
        current_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        texture_id,
        0
    );

    glGenRenderbuffers(1, &render_buffer_object_id);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object_id);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        current_width,
        current_height
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        render_buffer_object_id
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Resolve framebuffer not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FrameBuffer::FrameBuffer(
    size_t width,
    size_t height
)
    : current_height(height),
      current_width(width) {
    initialize();
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &frame_buffer_object_id);
    glDeleteTextures(1, &texture_id);
    glDeleteRenderbuffers(1, &render_buffer_object_id);

    glDeleteFramebuffers(1, &msaa_framebuffer_id);
    glDeleteTextures(1, &msaa_color_buffer_id);
    glDeleteRenderbuffers(1, &msaa_depth_buffer_id);
}

uint32_t FrameBuffer::frame_texture() {
    return texture_id;
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, msaa_framebuffer_id);
    glViewport(0, 0, current_width, current_height);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resolve() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa_framebuffer_id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer_object_id);
    glBlitFramebuffer(
        0,
        0,
        current_width,
        current_height,
        0,
        0,
        current_width,
        current_height,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::rescale(
    size_t width,
    size_t height
) {
    if (width == current_width && height == current_height) {
        return;
    }

    current_width = width;
    current_height = height;

    // Realloc MSAA targets
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaa_color_buffer_id);
    glTexImage2DMultisample(
        GL_TEXTURE_2D_MULTISAMPLE,
        samples,
        GL_RGB8,
        width,
        height,
        GL_TRUE
    );

    glBindRenderbuffer(GL_RENDERBUFFER, msaa_depth_buffer_id);
    glRenderbufferStorageMultisample(
        GL_RENDERBUFFER,
        samples,
        GL_DEPTH24_STENCIL8,
        width,
        height
    );

    // Realloc resolve target
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

double FrameBuffer::aspect() const {
    return static_cast<double>(current_width) /
           static_cast<double>(current_height);
}

size_t FrameBuffer::width() const {
    return current_width;
}

size_t FrameBuffer::height() const {
    return current_height;
}

}  // namespace slamd
