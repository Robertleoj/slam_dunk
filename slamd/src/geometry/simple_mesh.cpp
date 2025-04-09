#include <slamd/assert.hpp>
#include <slamd/geometry/simple_mesh.hpp>
#include <slamd/paths.hpp>

namespace slamd {
namespace _geometry {

const fs::path vertex_shader_path =
    shader_folder() / "simple_mesh" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "simple_mesh" / "fragment_shader.frag";

thread_local std::optional<ShaderProgram> SimpleMesh::shader;

void SimpleMesh::initialize() {
    assert_thread(this->render_thread_id.value());

    if (!SimpleMesh::shader.has_value()) {
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
        vertices.size() * sizeof(Vertex),
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
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );
    gl::glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    gl::glVertexAttribPointer(
        1,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
    );
    gl::glEnableVertexAttribArray(1);

    // unbind the vao
    gl::glBindVertexArray(0);

    this->gl_data.emplace(gl_data);
}

SimpleMesh::SimpleMesh(
    std::vector<Vertex> vertices,
    std::vector<uint32_t> triangle_indices
)
    : vertices(vertices),
      triangle_indices(triangle_indices) {}

void SimpleMesh::render(
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

    if (!SimpleMesh::shader.has_value()) {
        throw std::runtime_error("Shader not initialized!");
    }

    auto& shader = SimpleMesh::shader.value();

    shader.use();
    shader.setUniform("model", model);
    shader.setUniform("view", view);
    shader.setUniform("projection", projection);
    gl::glDrawElements(
        gl::GL_TRIANGLES, this->triangle_indices.size(), gl::GL_UNSIGNED_INT, 0
    );
    gl::glBindVertexArray(0);
};

}  // namespace _geometry

namespace geometry {

std::shared_ptr<SimpleMesh> simple_mesh(
    std::vector<_geometry::Vertex> vertices,
    std::vector<uint32_t> triangle_indices
) {
    return std::make_shared<SimpleMesh>(vertices, triangle_indices);
}

}  // namespace geometry

}  // namespace slamd