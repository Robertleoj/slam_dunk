#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/assert.hpp>
#include <slamd/geom/arcball_indicator.hpp>
#include <slamd/paths.hpp>
#include <slamd/render_thread_job_queue.hpp>

namespace slamd {
namespace _geom {

const fs::path vertex_shader_path =
    shader_folder() / "arcball_indicator" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "arcball_indicator" / "fragment_shader.frag";

void ArcballIndicator::initialize() {
    assert_thread(this->render_thread_id.value());
    // clang-format off
    std::vector<float> verts = {
        // x cross
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // y cross
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // z bar
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    // clang-format on

    this->vertex_count = verts.size() / 3;

    gl::GLuint vao_id;
    gl::GLuint vbo_id;

    gl::glGenVertexArrays(1, &vao_id);
    gl::glGenBuffers(1, &vbo_id);

    gl::glBindVertexArray(vao_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vbo_id);
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        verts.size() * sizeof(float),
        verts.data(),
        gl::GL_STATIC_DRAW
    );

    gl::glEnableVertexAttribArray(0);
    gl::glVertexAttribPointer(
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    gl::glBindVertexArray(0);

    GLData gl_state = {
        vao_id,
        vbo_id,
        ShaderProgram(vertex_shader_path, fragment_shader_path)
    };

    this->gl_state.emplace(gl_state);
}

ArcballIndicator::ArcballIndicator() {}

float ArcballIndicator::get_alpha() {
    // smoothly decrease alpha until 0
    // should take ~1sec to completely fade
    if (!this->last_interacted.has_value()) {
        return 0.0f;
    }

    auto now = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> sec_duration = now - last_interacted.value();
    float secs = sec_duration.count();

    float start_fading = 0.2f;
    float fade_speed = 0.2f;

    return glm::clamp(1.0f - ((secs - start_fading) / fade_speed), 0.0f, 1.0f);
}

void ArcballIndicator::interact() {
    this->last_interacted = std::chrono::high_resolution_clock::now();
}

glm::mat4 ArcballIndicator::get_scale_mat(
    float scale
) {
    return glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
}

void ArcballIndicator::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    if (!this->render_thread_id.has_value()) {
        this->render_thread_id = std::this_thread::get_id();
        this->initialize();
    }

    auto gl_state = this->gl_state.value().get();

    auto alpha = this->get_alpha();
    if (alpha < 1e-6) {
        // no need to draw
        return;
    }

    auto scale_mat =
        ArcballIndicator::get_scale_mat(this->arcball_zoom / 20.0f);
    gl_state->shader.use();
    gl_state->shader.set_uniform("uModel", model * scale_mat);
    gl_state->shader.set_uniform("uView", view);
    gl_state->shader.set_uniform("uProjection", projection);
    gl_state->shader.set_uniform(
        "uColor",
        glm::vec3(1.0f, 1.0f, 1.0f)
    );  // clean gray
    gl_state->shader.set_uniform("uAlpha", alpha);

    gl::glBindVertexArray(gl_state->vao_id);
    gl::glDrawArrays(gl::GL_LINES, 0, this->vertex_count);

    gl::glBindVertexArray(0);
};

void ArcballIndicator::set_arcball_zoom(
    float zoom
) {
    this->arcball_zoom = zoom;
}

}  // namespace _geom
}  // namespace slamd