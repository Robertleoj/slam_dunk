#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/assert.hpp>
#include <slamd/geometry/xy_grid.hpp>
#include <slamd/paths.hpp>
#include <slamd/render_thread_job_queue.hpp>

namespace slamd {
namespace geometry {

const fs::path vertex_shader_path =
    shader_folder() / "xy_grid" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "xy_grid" / "fragment_shader.frag";

glm::mat4 get_scale_mat(
    float log_scale
) {
    float scale = pow(10.0f, log_scale);
    spdlog::debug("Scaling by {}", scale);
    return glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 0.0));
}

void GridXYPlane::initialize() {
    assert_thread(this->render_thread_id.value());

    std::vector<float> verts;

    for (float x = -this->grid_size; x <= this->grid_size; x++) {
        verts.insert(
            verts.end(), {x, -this->grid_size, 0.0f, x, this->grid_size, 0.0f}
        );
    }

    for (float y = -this->grid_size; y <= this->grid_size; y++) {
        verts.insert(
            verts.end(), {-this->grid_size, y, 0.0f, this->grid_size, y, 0.0f}
        );
    }

    size_t vertex_count = verts.size() / 3;

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
        0, 3, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(float), (void*)0
    );

    gl::glBindVertexArray(0);

    GLData gl_data{
        vao_id,
        vbo_id,
        ShaderProgram(vertex_shader_path, fragment_shader_path),
        vertex_count
    };

    this->gl_data.emplace(gl_data);
}

GridXYPlane::GridXYPlane(
    float grid_size
)
    : grid_size(grid_size) {}

float calculate_alpha(
    float log_best,
    float level
) {
    return glm::clamp(1.0f - std::fabs(log_best - level), 0.0f, 1.0f);
}

void GridXYPlane::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    if (!this->render_thread_id.has_value()) {
        this->render_thread_id = std::this_thread::get_id();
        this->initialize();
    }

    auto gl_data = this->gl_data.value().get();

    float desired_scale = this->arcball_zoom / 10.0;

    float log_desired_scale = log10(desired_scale);

    // render three grids - at least one with the closest scale
    float best_log_scale = round(log_desired_scale);
    float log_scale_below = best_log_scale - 1;
    float log_scale_above = best_log_scale + 1;

    float best_alpha = calculate_alpha(log_desired_scale, best_log_scale);
    float below_alpha = calculate_alpha(log_desired_scale, log_scale_below);
    float above_alpha = calculate_alpha(log_desired_scale, log_scale_above);

    glm::mat4 best_scale_mat = get_scale_mat(best_log_scale);
    glm::mat4 below_scale_mat = get_scale_mat(log_scale_below);
    glm::mat4 above_scale_mat = get_scale_mat(log_scale_above);

    // Assuming your shader is already bound
    // And has these uniforms: uModel, uView, uProjection, uColor
    gl_data->shader.use();
    gl_data->shader.setUniform("uView", view);
    gl_data->shader.setUniform("uProjection", projection);
    gl_data->shader.setUniform(
        "uColor", glm::vec3(0.6f, 0.6f, 0.6f)
    );  // clean gray

    // render best scale
    gl_data->shader.setUniform("uModel", model * best_scale_mat);
    gl_data->shader.setUniform("uExtraAlpha", best_alpha);
    gl_data->shader.setUniform("uScale", desired_scale);

    gl::glBindVertexArray(gl_data->vao_id);
    gl::glDepthMask(gl::GL_FALSE);  // don't mess with z-buffer

    gl::glDrawArrays(gl::GL_LINES, 0, gl_data->vertex_count);

    float draw_threshold = 0.01;

    spdlog::debug("best: {} below: {}", best_alpha, below_alpha);

    if (below_alpha > draw_threshold) {
        gl_data->shader.setUniform("uModel", model * below_scale_mat);
        gl_data->shader.setUniform("uExtraAlpha", below_alpha);

        gl::glDrawArrays(gl::GL_LINES, 0, gl_data->vertex_count);
    }
    if (above_alpha > draw_threshold) {
        gl_data->shader.setUniform("uModel", model * above_scale_mat);
        gl_data->shader.setUniform("uExtraAlpha", above_alpha);

        gl::glDrawArrays(gl::GL_LINES, 0, gl_data->vertex_count);
    }
    gl::glDepthMask(gl::GL_TRUE);

    gl::glBindVertexArray(0);
}

GridXYPlane::~GridXYPlane() {
    if (!this->render_thread_id.has_value()) {
        return;
    }

    auto gl_data = this->gl_data.value().get();

    gl::GLuint vbo_id = gl_data->vbo_id;
    auto vao_id = gl_data->vao_id;

    auto job = [vbo_id, vao_id]() {
        gl::glDeleteBuffers(1, &vbo_id);
        gl::glDeleteVertexArrays(1, &vao_id);
    };

    RenderQueueManager::force_enqueue(this->render_thread_id.value(), job);
}

void GridXYPlane::set_arcball_zoom(
    float zoom
) {
    this->arcball_zoom = zoom;
}

}  // namespace geometry
}  // namespace slamd