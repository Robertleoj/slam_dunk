#include <slamd/assert.hpp>
#include <slamd/constants.hpp>
#include <slamd/geom/mono_mesh.hpp>
#include <slamd/geom/utils.hpp>
#include <slamd/paths.hpp>

namespace slamd {
namespace _geom {

const fs::path vertex_shader_path =
    shader_folder() / "mono_mesh" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "mono_mesh" / "fragment_shader.frag";

thread_local std::optional<ShaderProgram> MonoMesh::shader;

void MonoMesh::initialize() {
    assert_thread(this->render_thread_id.value());

    if (!MonoMesh::shader.has_value()) {
        shader.emplace(vertex_shader_path, fragment_shader_path);
    }

    GLData gl_data;

    gl::glGenVertexArrays(1, &gl_data.vao_id);
    gl::glBindVertexArray(gl_data.vao_id);

    // Vertex positions
    gl::glGenBuffers(1, &gl_data.vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, gl_data.vbo_id);
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        mesh_data.vertices.size() * sizeof(data::Vertex),
        mesh_data.vertices.data(),
        gl::GL_STATIC_DRAW
    );
    gl::glVertexAttribPointer(
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(data::Vertex),
        (void*)0
    );
    gl::glEnableVertexAttribArray(0);

    gl::glVertexAttribPointer(
        1,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(data::Vertex),
        (void*)offsetof(data::Vertex, normal)
    );
    gl::glEnableVertexAttribArray(1);

    // Indices
    gl::glGenBuffers(1, &gl_data.eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, gl_data.eab_id);
    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        mesh_data.triangle_indices.size() * sizeof(uint32_t),
        mesh_data.triangle_indices.data(),
        gl::GL_STATIC_DRAW
    );

    gl::glBindVertexArray(0);

    this->gl_data.emplace(gl_data);
}

MonoMesh::MonoMesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<uint32_t>& triangle_indices,
    const glm::vec3& color,
    float min_brightness
)
    : min_brightness(min_brightness),
      color(color),
      mesh_data(make_mesh(vertices, triangle_indices)) {}

MonoMesh::MonoMesh(
    const data::Mesh& mesh_data,
    const glm::vec3& color,
    float min_brightness
)
    : color(color),
      min_brightness(min_brightness),
      mesh_data(mesh_data) {}

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
    shader.set_uniform("model", model);
    shader.set_uniform("view", view);
    shader.set_uniform("projection", projection);
    shader.set_uniform("color", this->color);
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

std::shared_ptr<MonoMesh> mono_mesh(
    std::vector<glm::vec3> vertices,
    std::vector<uint32_t> triangle_indices,
    glm::vec3 color
) {
    return std::make_shared<MonoMesh>(vertices, triangle_indices, color);
}

}  // namespace geom
}  // namespace slamd
