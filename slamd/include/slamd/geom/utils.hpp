#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <slamd/data/mesh.hpp>
#include <vector>

namespace slamd {
namespace _geom {

size_t generate_sphere(
    std::vector<glm::vec3>& vertices,
    std::vector<uint32_t>& indices,
    std::vector<glm::vec3>& normals,
    float radius,
    uint sectorCount,
    uint stackCount
);

std::vector<glm::vec3> compute_vertex_normals(
    const std::vector<glm::vec3>& vertex_positions,
    const std::vector<uint32_t>& triangle_indices
);

data::ColoredMesh make_colored_mesh(
    const std::vector<glm::vec3>& vertex_positions,
    const std::vector<glm::vec3>& vertex_colors,
    const std::vector<uint32_t>& triangle_indices
);

data::ColoredMesh make_colored_mesh(
    const std::vector<glm::vec3>& vertex_positions,
    const glm::vec3& color,
    const std::vector<uint32_t>& triangle_indices
);

data::ColoredMesh make_colored_mesh(
    const std::vector<glm::vec3>& vertex_positions,
    const std::vector<uint32_t>& triangle_indices,
    const glm::vec3& color,
    const std::vector<glm::vec3>& normals
);

data::ColoredMesh
make_colored_mesh(const data::Mesh& mesh_data, const glm::vec3& color);

data::Mesh make_mesh(
    const std::vector<glm::vec3>& vertex_positions,
    const std::vector<uint32_t>& triangle_indices,
    const std::vector<glm::vec3>& normals
);

data::Mesh make_mesh(
    const std::vector<glm::vec3>& vertex_positions,
    const std::vector<uint32_t>& triangle_indices
);

}  // namespace _geom
}  // namespace slamd
