#include <glm/gtc/matrix_transform.hpp>
#include <slam_dunk/paths.hpp>
#include <slam_dunk/scene_objects/arcball_indicator.hpp>

namespace sdunk {

const fs::path vertex_shader_path =
    shader_folder() / "arcball_indicator" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "arcball_indicator" / "fragment_shader.frag";

ArcballIndicator::ArcballIndicator()
    : vao_id(0),
      vbo_id(0),
      vertex_count(0),
      shader(vertex_shader_path, fragment_shader_path) {
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
    auto scale_mat =
        ArcballIndicator::get_scale_mat(this->arcball_zoom / 20.0f);
    this->shader.use();
    this->shader.setUniform("uModel", model * scale_mat);
    this->shader.setUniform("uView", view);
    this->shader.setUniform("uProjection", projection);
    this->shader.setUniform(
        "uColor", glm::vec3(1.0f, 1.0f, 1.0f)
    );  // clean gray

    gl::glBindVertexArray(this->vao_id);
    gl::glDrawArrays(gl::GL_LINES, 0, this->vertex_count);

    gl::glBindVertexArray(0);
};

void ArcballIndicator::set_arcball_zoom(
    float zoom
) {
    this->arcball_zoom = zoom;
}

}  // namespace sdunk