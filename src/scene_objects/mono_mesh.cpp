#include <slam_dunk/paths.hpp>
#include <slam_dunk/scene_objects/mono_mesh.hpp>

namespace sdunk {

const fs::path vertex_shader_path =
    shader_folder() / "mono_mesh" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "mono_mesh" / "fragment_shader.frag";

std::optional<ShaderProgram> MonoMesh::shader;

MonoMesh::MonoMesh(
    std::vector<glm::vec3> vertices,
    std::vector<uint32_t> triangle_indices,
    glm::vec3 color
)
    : num_vertices(triangle_indices.size()),
      color(color) {
    if (!MonoMesh::shader.has_value()) {
        shader.emplace(vertex_shader_path, fragment_shader_path);
    }

    // create the vertex array object
    gl::glGenVertexArrays(1, &this->vao_id);
    gl::glBindVertexArray(this->vao_id);

    // make the vertex buffer object
    gl::glGenBuffers(1, &this->vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->vbo_id);
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
        gl::GL_STATIC_DRAW
    );

    // make the element array buffer
    gl::glGenBuffers(1, &this->eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, this->eab_id);

    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        triangle_indices.size() * sizeof(uint32_t),
        triangle_indices.data(),
        gl::GL_STATIC_DRAW
    );

    // Position attribute (location = 0)
    gl::glVertexAttribPointer(
        0, 3, gl::GL_FLOAT, gl::GL_FALSE, sizeof(glm::vec3), (void*)0
    );
    gl::glEnableVertexAttribArray(0);

    // unbind the vao
    gl::glBindVertexArray(0);
}

void MonoMesh::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    gl::glBindVertexArray(this->vao_id);

    if (!MonoMesh::shader.has_value()) {
        throw std::runtime_error("Shader not initialized!");
    }

    auto& shader = MonoMesh::shader.value();

    shader.use();
    shader.setUniform("model", model);
    shader.setUniform("view", view);
    shader.setUniform("projection", projection);
    shader.setUniform("color", this->color);
    gl::glDrawElements(
        gl::GL_TRIANGLES, this->num_vertices, gl::GL_UNSIGNED_INT, 0
    );
    gl::glBindVertexArray(0);
};
}  // namespace sdunk