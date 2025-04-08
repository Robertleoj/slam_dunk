#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <spdlog/spdlog.h>
#include <slamd/assert.hpp>
#include <slamd/frame_buffer.hpp>
#include <slamd/render_thread_job_queue.hpp>

namespace slamd {

void FrameBuffer::initialize() {
    assert_thread(this->render_thread_id.value());
    assert_nil(this->gl_data);
    spdlog::debug("Calling initialize");

    GLData gl_data;

    gl::glGenFramebuffers(1, &gl_data.frame_buffer_object_id);
    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, gl_data.frame_buffer_object_id);

    gl::glGenTextures(1, &gl_data.texture_id);
    gl::glBindTexture(gl::GL_TEXTURE_2D, gl_data.texture_id);
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
        gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR
    );
    gl::glTexParameteri(
        gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR
    );

    gl::glFramebufferTexture2D(
        gl::GL_FRAMEBUFFER,
        gl::GL_COLOR_ATTACHMENT0,
        gl::GL_TEXTURE_2D,
        gl_data.texture_id,
        0
    );

    gl::glGenRenderbuffers(1, &gl_data.render_buffer_object_id);
    gl::glBindRenderbuffer(
        gl::GL_RENDERBUFFER, gl_data.render_buffer_object_id
    );
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
        gl_data.render_buffer_object_id
    );

    if (gl::glCheckFramebufferStatus(gl::GL_FRAMEBUFFER) !=
        gl::GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Framebuffer is not complete!");
        throw std::runtime_error("Framebuffer not complete");
    }

    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
    gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
    gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, 0);

    this->gl_data.emplace(gl_data);
}

FrameBuffer::FrameBuffer(
    size_t width,
    size_t height
) {
    this->current_height = height;
    this->current_width = width;
}

FrameBuffer::GLData* FrameBuffer::get_gl_data() {
    this->set_render_thread();
    return this->gl_data.value().get();
}

FrameBuffer::~FrameBuffer() {
    auto gl_data = this->get_gl_data();

    auto job = [fbo = gl_data->frame_buffer_object_id,
                rbo = gl_data->render_buffer_object_id,
                tex = gl_data->texture_id]() {
        gl::glDeleteFramebuffers(1, &fbo);
        gl::glDeleteTextures(1, &tex);
        gl::glDeleteRenderbuffers(1, &rbo);
    };

    RenderQueueManager::force_enqueue(this->render_thread_id.value(), job);
}

gl::GLuint FrameBuffer::frame_texture() {
    auto gl_data = this->get_gl_data();
    return gl_data->texture_id;
}

void FrameBuffer::rescale(
    size_t width,
    size_t height
) {
    if (this->current_height == height && this->current_width == width) {
        return;
    }

    auto gl_data = this->get_gl_data();

    spdlog::debug("Resizing to {} {}", width, height);
    gl::glViewport(0, 0, width, height);

    gl::glBindTexture(gl::GL_TEXTURE_2D, gl_data->texture_id);
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
        gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR
    );

    gl::glTexParameteri(
        gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR
    );
    gl::glFramebufferTexture2D(
        gl::GL_FRAMEBUFFER,
        gl::GL_COLOR_ATTACHMENT0,
        gl::GL_TEXTURE_2D,
        gl_data->texture_id,
        0
    );

    gl::glBindRenderbuffer(
        gl::GL_RENDERBUFFER, gl_data->render_buffer_object_id
    );
    gl::glRenderbufferStorage(
        gl::GL_RENDERBUFFER, gl::GL_DEPTH24_STENCIL8, width, height
    );
    gl::glFramebufferRenderbuffer(
        gl::GL_FRAMEBUFFER,
        gl::GL_DEPTH_STENCIL_ATTACHMENT,
        gl::GL_RENDERBUFFER,
        gl_data->render_buffer_object_id
    );

    this->current_height = height;
    this->current_width = width;
}

void FrameBuffer::bind() {
    auto gl_data = this->get_gl_data();
    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, gl_data->frame_buffer_object_id);
}

void FrameBuffer::unbind() {
    // make sure we can
    this->get_gl_data();

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

void FrameBuffer::set_render_thread() {
    if (this->render_thread_id.has_value()) {
        assert_thread(this->render_thread_id.value());
        return;
    }
    this->render_thread_id = std::this_thread::get_id();
    this->initialize();
}

}  // namespace slamd