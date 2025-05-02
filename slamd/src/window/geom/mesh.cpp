#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <slamd_common/utils/mesh.hpp>
#include <slamd_window/assert.hpp>
#include <slamd_window/constants.hpp>
#include <slamd_window/gen/shader_sources.hpp>
#include <slamd_window/geom/mesh.hpp>
#include <slamd_window/paths.hpp>

namespace slamd {
namespace _geom {

std::shared_ptr<Mesh> Mesh::deserialize(
    const slamd::flatb::Mesh* mesh_fb
) {
    return std::make_shared<Mesh>(
        slamd::data::MeshData::deserialize(mesh_fb->data()),
        mesh_fb->min_brightness()
    );
}

thread_local std::optional<ShaderProgram> Mesh::shader;

void Mesh::initialize() {
    if (!Mesh::shader.has_value()) {
        Mesh::shader.emplace(
            ShaderProgram(shader_source::mesh::vert, shader_source::mesh::frag)
        );
    }

    // create the vertex array object
    gl::glGenVertexArrays(1, &this->vao_id);
    gl::glBindVertexArray(this->vao_id);

    // positions
    gl::glGenBuffers(1, &this->pos_vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->pos_vbo_id);
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
    gl::glGenBuffers(1, &this->color_vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->color_vbo_id);
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
    gl::glGenBuffers(1, &this->normal_vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->normal_vbo_id);
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
    gl::glGenBuffers(1, &this->eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, this->eab_id);

    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        this->mesh_data.triangle_indices.size() * sizeof(uint32_t),
        this->mesh_data.triangle_indices.data(),
        gl::GL_STATIC_DRAW
    );

    // unbind the vao
    gl::glBindVertexArray(0);
}

void Mesh::update_positions(
    const std::vector<glm::vec3>& positions
) {
    if (positions.size() != this->mesh_data.positions.size()) {
        throw std::invalid_argument(
            "New positions must have the same size as original"
        );
    }

    this->mesh_data.positions = positions;
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
    if (this->pos_update_pending) {
        gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->pos_vbo_id);
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->mesh_data.positions.size() * sizeof(glm::vec3),
            this->mesh_data.positions.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->pos_update_pending = false;
    }

    if (this->color_update_pending) {
        gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->color_vbo_id);
        gl::glBufferData(
            gl::GL_ARRAY_BUFFER,
            this->mesh_data.colors.size() * sizeof(glm::vec3),
            this->mesh_data.colors.data(),
            gl::GL_DYNAMIC_DRAW
        );
        this->color_update_pending = false;
    }

    if (this->normal_update_pending) {
        gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->normal_vbo_id);
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
    const slamd::data::MeshData& mesh_data,
    float min_brightness
)
    : mesh_data(mesh_data),
      min_brightness(min_brightness) {
    this->initialize();
}

Mesh::Mesh(
    slamd::data::MeshData&& mesh_data,
    float min_brightness
)
    : mesh_data(std::move(mesh_data)),
      min_brightness(min_brightness) {
    this->initialize();
}

void Mesh::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    gl::glBindVertexArray(this->vao_id);

    this->handle_updates();

    if (!Mesh::shader.has_value()) {
        throw std::runtime_error("Mesh shader not initialized!");
    }

    auto& shader = Mesh::shader.value();

    shader.use();
    shader.set_uniform("model", model);
    shader.set_uniform("view", view);
    shader.set_uniform("projection", projection);
    shader.set_uniform("light_dir", _const::light_dir);
    shader.set_uniform("min_brightness", this->min_brightness);
    gl::glDrawElements(
        gl::GL_TRIANGLES,
        this->mesh_data.triangle_indices.size(),
        gl::GL_UNSIGNED_INT,
        0
    );
    gl::glBindVertexArray(0);
};

Mesh::~Mesh() {
    gl::glDeleteBuffers(1, &eab_id);
    gl::glDeleteBuffers(1, &pos_vbo_id);
    gl::glDeleteBuffers(1, &color_vbo_id);
    gl::glDeleteBuffers(1, &normal_vbo_id);
    gl::glDeleteVertexArrays(1, &vao_id);
}

}  // namespace _geom

namespace geom {

MeshPtr mesh(
    const slamd::data::MeshData& mesh_data
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