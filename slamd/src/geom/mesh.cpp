#include <ranges>
#include <slamd/assert.hpp>
#include <slamd/constants.hpp>
#include <slamd/geom/mesh.hpp>
#include <slamd/geom/utils.hpp>

namespace slamd {
namespace _geom {

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
    return;
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