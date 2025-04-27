#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <slamd/constants.hpp>
#include <slamd/data/mesh.hpp>
#include <slamd/geom/geometry.hpp>
#include <vector>

namespace slamd {
namespace _geom {

class Mesh : public Geometry {
   public:
    Mesh(
        const data::MeshData& mesh_data,
        float min_brightness = _const::default_min_brightness
    );

    Mesh(
        data::MeshData&& mesh_data,
        float min_brightness = _const::default_min_brightness
    );

    void update_positions(
        const std::vector<glm::vec3>& positions,
        bool recompute_normals = true
    );
    void update_colors(const std::vector<glm::vec3>& colors);
    void update_normals(const std::vector<glm::vec3>& normals);
};

}  // namespace _geom

namespace geom {

using MeshPtr = std::shared_ptr<_geom::Mesh>;

MeshPtr mesh(const data::MeshData& mesh_data);

}  // namespace geom
}  // namespace slamd