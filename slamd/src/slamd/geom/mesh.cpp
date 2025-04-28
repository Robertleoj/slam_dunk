#include <ranges>
#include <slamd/assert.hpp>
#include <slamd/constants.hpp>
#include <slamd/geom/mesh.hpp>
#include <slamd_common/data/mesh.hpp>

namespace slamd {
namespace _geom {

void Mesh::update_positions(
    const std::vector<glm::vec3>& positions,
    bool recompute_normals
) {};

void Mesh::update_colors(const std::vector<glm::vec3>& colors) {};

void Mesh::update_normals(const std::vector<glm::vec3>& normals) {};

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

flatbuffers::Offset<slamd::flatb::Geometry> Mesh::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto mesh_data_fb = this->mesh_data.serialize(builder);
    auto mesh_fb =
        flatb::CreateMesh(builder, mesh_data_fb, this->min_brightness);

    return flatb::CreateGeometry(
        builder,
        flatb::GeometryUnion_mesh,
        mesh_fb.Union()
    );
}

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