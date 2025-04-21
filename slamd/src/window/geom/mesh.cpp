#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <ranges>
#include <slamd/assert.hpp>
#include <slamd/constants.hpp>
#include <slamd/gen/shader_sources.hpp>
#include <slamd/geom/mesh.hpp>
#include <slamd/geom/utils.hpp>
#include <slamd/paths.hpp>

namespace slamd {
namespace _geom {

thread_local std::optional<ShaderProgram> Mesh::shader;

void Mesh::maybe_initialize() {
    if (this->render_thread_id.has_value()) {
        assert_thread(this->render_thread_id.value());
        return;
    }

    this->render_thread_id = std::this_thread::get_id();

    if (!Mesh::shader.has_value()) {
        shader.emplace(shader_source::mesh::vert, shader_source::mesh::frag);
    }

    GLData gl_data;

    // create the vertex array object
    gl::glGenVertexArrays(1, &gl_data.vao_id);
    gl::glBindVertexArray(gl_data.vao_id);

    // positions
    gl::glGenBuffers(1, &gl_data.pos_vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, gl_data.pos_vbo_id);
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        this->mesh_data.positions.size() * sizeof(glm::vec3),
        this->mesh_data.positions.data(),
        gl::GL_DYNAMIC_DRAW
    );

    gl::glVertexAttribPointer(
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );
    gl::glEnableVertexAttribArray(0);

    // colors
    gl::glGenBuffers(1, &gl_data.color_vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, gl_data.color_vbo_id);
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        this->mesh_data.colors.size() * sizeof(glm::vec3),
        this->mesh_data.colors.data(),
        gl::GL_DYNAMIC_DRAW
    );

    gl::glVertexAttribPointer(
        1,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );
    gl::glEnableVertexAttribArray(1);

    // /normals
    gl::glGenBuffers(1, &gl_data.normal_vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, gl_data.normal_vbo_id);
    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        this->mesh_data.normals.size() * sizeof(glm::vec3),
        this->mesh_data.normals.data(),
        gl::GL_DYNAMIC_DRAW
    );

    gl::glVertexAttribPointer(
        2,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );
    gl::glEnableVertexAttribArray(2);

    // make the element array buffer
    gl::glGenBuffers(1, &gl_data.eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, gl_data.eab_id);

    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        this->mesh_data.triangle_indices.size() * sizeof(uint32_t),
        this->mesh_data.triangle_indices.data(),
        gl::GL_STATIC_DRAW
    );

    // unbind the vao
    gl::glBindVertexArray(0);

    this->gl_data.emplace(gl_data);
}

void Mesh::update_positions(
    const std::vector<glm::vec3>& positions,
    bool recompute_normals
) {
    if (positions.size() != this->mesh_data.positions.size()) {
        throw std::invalid_argument(
            "New positions must have the same size as original"
        );
    }

    this->mesh_data.positions = positions;
    if (recompute_normals) {
        this->mesh_data.recompute_normals();
        this->normal_update_pending = true;
    }
    this->pos_update_pending = true;
};
void Mesh::update_colors(
    const std::vector<glm::vec3>& colors
) {
    if (colors.size() != this->mesh_data.colors.size()) {
        throw std::invalid_argument("Size of new colors should match existing");
    }

    this->mesh_data.colors = colors;
    this->color_update_pending = true;
};
void Mesh::update_normals(
    const std::vector<glm::vec3>& normals
) {
    if (normals.size() != this->mesh_data.normals.size()) {
        throw std::invalid_argument("Size of new colors should match existing");
    }

    this->mesh_data.normals = normals;
    this->normal_update_pending = true;
};

void Mesh::handle_updates() {
    auto& gl_data = this->gl_data.value();
    if (this->pos_update_pending) {
        gl::glBindBuffer(gl::GL_ARRAY_BUFFER, gl_data.pos_vbo_id);
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->mesh_data.positions.size() * sizeof(glm::vec3),
            this->mesh_data.positions.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->pos_update_pending = false;
    }

    if (this->color_update_pending) {
        gl::glBindBuffer(gl::GL_ARRAY_BUFFER, gl_data.color_vbo_id);
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->mesh_data.colors.size() * sizeof(glm::vec3),
            this->mesh_data.colors.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->color_update_pending = false;
    }

    if (this->normal_update_pending) {
        gl::glBindBuffer(gl::GL_ARRAY_BUFFER, gl_data.normal_vbo_id);
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->mesh_data.normals.size() * sizeof(glm::vec3),
            this->mesh_data.normals.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->normal_update_pending = false;
    }
}

Mesh::Mesh(
    const data::MeshData& mesh_data,
    float min_brightness
)
    : mesh_data(mesh_data),
      min_brightness(min_brightness) {}

Mesh::Mesh(
    data::MeshData&& mesh_data,
    float min_brightness
)
    : mesh_data(std::move(mesh_data)),
      min_brightness(min_brightness) {}

void Mesh::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->maybe_initialize();
    auto& gl_data = this->gl_data.value();

    gl::glBindVertexArray(gl_data.vao_id);

    this->handle_updates();

    if (!Mesh::shader.has_value()) {
        throw std::runtime_error("Shader not initialized!");
    }

    auto& shader = Mesh::shader.value();

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

MeshPtr mesh(
    const data::MeshData& mesh_data
) {
    return std::make_shared<_geom::Mesh>(mesh_data);
}

// std::shared_ptr<Mesh> mesh(
//     const std::vector<glm::vec3>& vertices,
//     const std::vector<glm::vec3>& vertex_colors,
//     const std::vector<uint32_t>& triangle_indices
// ) {
//     data::MeshData data = data::MeshDataBuilder()
//                               .set_positions(vertices)
//                               .set_colors(vertex_colors)
//                               .set_indices(triangle_indices)
//                               .compute_normals()
//                               .build();

//     return std::make_shared<Mesh>(data);
// }
// std::shared_ptr<Mesh> mesh(
//     const std::vector<glm::vec3>& vertices,
//     const std::vector<glm::vec3>& vertex_colors,
//     const std::vector<uint32_t>& triangle_indices,
//     const std::vector<glm::vec3>& normals
// ) {
//     data::MeshData data = data::MeshDataBuilder()
//                               .set_positions(vertices)
//                               .set_colors(vertex_colors)
//                               .set_indices(triangle_indices)
//                               .set_normals(normals)
//                               .build();

//     return std::make_shared<Mesh>(vertices, vertex_colors, triangle_indices);
// }

}  // namespace geom

}  // namespace slamd