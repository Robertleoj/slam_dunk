#include <slam_dunk/paths.hpp>
#include <slam_dunk/scene_objects/xy_grid.hpp>

namespace sdunk {

const fs::path vertex_shader_path =
    shader_folder() / "xy_grid" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "xy_grid" / "fragment_shader.frag";

GridXYPlane::GridXYPlane(
    float grid_size,
    float spacing
)
    : vao_id(0),
      vbo_id(0),
      vertex_count(0),
      shader(vertex_shader_path, fragment_shader_path) {
    std::vector<float> verts;

    for (float x = -grid_size; x <= grid_size; x += spacing) {
        verts.insert(verts.end(), {x, -grid_size, 0.0f, x, grid_size, 0.0f});
    }

    for (float y = -grid_size; y <= grid_size; y += spacing) {
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

void GridXYPlane::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    // Assuming your shader is already bound
    // And has these uniforms: uModel, uView, uProjection, uColor
    this->shader.use();
    this->shader.setUniform("uModel", model);
    this->shader.setUniform("uView", view);
    this->shader.setUniform("uProjection", projection);
    this->shader.setUniform(
        "uColor", glm::vec3(0.6f, 0.6f, 0.6f)
    );  // clean gray

    gl::glBindVertexArray(this->vao_id);
    gl::glDrawArrays(gl::GL_LINES, 0, this->vertex_count);
    gl::glBindVertexArray(0);
}

GridXYPlane::~GridXYPlane() {
    gl::glDeleteBuffers(1, &this->vbo_id);
    gl::glDeleteVertexArrays(1, &this->vao_id);
}

}  // namespace sdunk