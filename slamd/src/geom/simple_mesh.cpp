#include <ranges>
#include <slamd/assert.hpp>
#include <slamd/constants.hpp>
#include <slamd/gen/shader_sources.hpp>
#include <slamd/geom/simple_mesh.hpp>
#include <slamd/geom/utils.hpp>
#include <slamd/paths.hpp>

namespace slamd {
namespace _geom {

thread_local std::optional<ShaderProgram> SimpleMesh::shader;

void SimpleMesh::initialize() {
    assert_thread(this->render_thread_id.value());

    if (!SimpleMesh::shader.has_value()) {
        shader.emplace(
            shader_source::simple_mesh::vert,
            shader_source::simple_mesh::frag
        );
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
        this->mesh_data.vertices.size() * sizeof(data::ColoredVertex),
        this->mesh_data.vertices.data(),
        gl::GL_STATIC_DRAW
    );

    // make the element array buffer
    gl::glGenBuffers(1, &gl_data.eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, gl_data.eab_id);

    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        this->mesh_data.triangle_indices.size() * sizeof(uint32_t),
        this->mesh_data.triangle_indices.data(),
        gl::GL_STATIC_DRAW
    );

    // Position attribute (location = 0)
    gl::glVertexAttribPointer(
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(data::ColoredVertex),
        (void*)offsetof(data::ColoredVertex, position)
    );
    gl::glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    gl::glVertexAttribPointer(
        1,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(data::ColoredVertex),
        (void*)offsetof(data::ColoredVertex, color)
    );
    gl::glEnableVertexAttribArray(1);

    // Normals
    gl::glVertexAttribPointer(
        2,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(data::ColoredVertex),
        (void*)offsetof(data::ColoredVertex, normal)
    );
    gl::glEnableVertexAttribArray(2);

    // unbind the vao
    gl::glBindVertexArray(0);

    this->gl_data.emplace(gl_data);
}

SimpleMesh::SimpleMesh(
    const data::ColoredMesh& mesh_data,
    float min_brightness
)
    : mesh_data(mesh_data),
      min_brightness(min_brightness) {}

SimpleMesh::SimpleMesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& vertex_colors,
    const std::vector<uint32_t>& triangle_indices,
    float min_brightness
)
    : min_brightness(min_brightness) {
    if (vertices.size() != vertex_colors.size()) {
        throw std::invalid_argument(
            "number of vertices must equal number of vertex colors"
        );
    }

    this->mesh_data =
        make_colored_mesh(vertices, vertex_colors, triangle_indices);
}

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
    shader.set_uniform("model", model);
    shader.set_uniform("view", view);
    shader.set_uniform("projection", projection);
    shader.set_uniform("light_dir", slamd::_const::light_dir);
    shader.set_uniform("min_brightness", this->min_brightness);
    gl::glDrawElements(
        gl::GL_TRIANGLES,
        this->mesh_data.triangle_indices.size(),
        gl::GL_UNSIGNED_INT,
        0
    );
    gl::glBindVertexArray(0);
};

}  // namespace _geom

namespace geom {

std::shared_ptr<SimpleMesh> simple_mesh(
    const data::ColoredMesh& mesh_data
) {
    return std::make_shared<SimpleMesh>(mesh_data);
}

std::shared_ptr<SimpleMesh> simple_mesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& vertex_colors,
    const std::vector<uint32_t>& triangle_indices
) {
    return std::make_shared<SimpleMesh>(
        vertices,
        vertex_colors,
        triangle_indices
    );
}

}  // namespace geom

}  // namespace slamd