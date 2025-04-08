#include <slamd/geometry/mono_mesh.hpp>
#include <slamd/paths.hpp>

namespace slamd {
namespace geometry {

const fs::path vertex_shader_path =
    shader_folder() / "mono_mesh" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "mono_mesh" / "fragment_shader.frag";

thread_local std::optional<ShaderProgram> MonoMesh::shader;

void MonoMesh::initialize() {
    if (!MonoMesh::shader.has_value()) {
        shader.emplace(vertex_shader_path, fragment_shader_path);
    }

    GLData gl_data;

    // create the vertex array object
    gl::glGenVertexArrays(1, &gl_data.vao_id);
    gl::glBindVertexArray(gl_data.vao_id);

    // make the vertex buffer object
    gl::glGenBuffers(1, &gl_data.vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, gl_data.vbo_id);
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
        gl::GL_STATIC_DRAW
    );

    // make the element array buffer
    gl::glGenBuffers(1, &gl_data.eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, gl_data.eab_id);

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

    this->gl_data.emplace(gl_data);
}

MonoMesh::MonoMesh(
    std::vector<glm::vec3> vertices,
    std::vector<uint32_t> triangle_indices,
    glm::vec3 color
)
    : vertices(vertices),
      triangle_indices(triangle_indices),
      color(color) {}

void MonoMesh::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    if (!this->render_thread_id.has_value()) {
        this->render_thread_id = std::this_thread::get_id();
        this->initialize();
    }

    auto gl_data = this->gl_data.value().get();

    gl::glBindVertexArray(gl_data->vao_id);

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
        gl::GL_TRIANGLES, this->triangle_indices.size(), gl::GL_UNSIGNED_INT, 0
    );
    gl::glBindVertexArray(0);
};

std::shared_ptr<MonoMesh> mono_mesh(
    std::vector<glm::vec3> vertices,
    std::vector<uint32_t> triangle_indices,
    glm::vec3 color
) {
    return std::make_shared<MonoMesh>(vertices, triangle_indices, color);
}

}  // namespace geometry
}  // namespace slamd