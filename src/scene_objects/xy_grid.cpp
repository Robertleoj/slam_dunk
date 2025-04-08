#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <slam_dunk/paths.hpp>
#include <slam_dunk/scene_objects/xy_grid.hpp>

namespace sdunk {

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

GridXYPlane::GridXYPlane(
    float grid_size
)
    : vao_id(0),
      vbo_id(0),
      vertex_count(0),
      shader(vertex_shader_path, fragment_shader_path) {
    std::vector<float> verts;

    for (float x = -grid_size; x <= grid_size; x++) {
        verts.insert(verts.end(), {x, -grid_size, 0.0f, x, grid_size, 0.0f});
    }

    for (float y = -grid_size; y <= grid_size; y++) {
        verts.insert(verts.end(), {-grid_size, y, 0.0f, grid_size, y, 0.0f});
    }

    this->vertex_count = verts.size() / 3;

    gl::glGenVertexArrays(1, &this->vao_id);
    gl::glGenBuffers(1, &this->vbo_id);

    gl::glBindVertexArray(this->vao_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->vbo_id);
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
}

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
    this->shader.use();
    this->shader.setUniform("uView", view);
    this->shader.setUniform("uProjection", projection);
    this->shader.setUniform(
        "uColor", glm::vec3(0.6f, 0.6f, 0.6f)
    );  // clean gray

    // render best scale
    this->shader.setUniform("uModel", model * best_scale_mat);
    this->shader.setUniform("uExtraAlpha", best_alpha);
    this->shader.setUniform("uScale", desired_scale);

    gl::glBindVertexArray(this->vao_id);
    gl::glDepthMask(gl::GL_FALSE);  // don't mess with z-buffer

    gl::glDrawArrays(gl::GL_LINES, 0, this->vertex_count);

    float draw_threshold = 0.01;

    spdlog::debug("best: {} below: {}", best_alpha, below_alpha);

    if (below_alpha > draw_threshold) {
        this->shader.setUniform("uModel", model * below_scale_mat);
        this->shader.setUniform("uExtraAlpha", below_alpha);

        gl::glDrawArrays(gl::GL_LINES, 0, this->vertex_count);
    }
    if (above_alpha > draw_threshold) {
        this->shader.setUniform("uModel", model * above_scale_mat);
        this->shader.setUniform("uExtraAlpha", above_alpha);

        gl::glDrawArrays(gl::GL_LINES, 0, this->vertex_count);
    }
    gl::glDepthMask(gl::GL_TRUE);

    gl::glBindVertexArray(0);
}

GridXYPlane::~GridXYPlane() {
    gl::glDeleteBuffers(1, &this->vbo_id);
    gl::glDeleteVertexArrays(1, &this->vao_id);
}

void GridXYPlane::set_arcball_zoom(
    float zoom
) {
    this->arcball_zoom = zoom;
}

}  // namespace sdunk