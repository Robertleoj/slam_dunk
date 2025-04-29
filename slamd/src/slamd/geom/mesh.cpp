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
    auto mesh = std::make_shared<_geom::Mesh>(mesh_data);
    // _global::geometries.add(mesh->id, mesh);
    return mesh;
}

}  // namespace geom

}  // namespace slamd