#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <slamd_common/data/mesh.hpp>
#include <vector>

namespace slamd {
namespace _utils {

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

}  // namespace _utils
}  // namespace slamd
