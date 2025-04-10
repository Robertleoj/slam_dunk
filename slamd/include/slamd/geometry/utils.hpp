#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace slamd {
namespace _geometry {

size_t generate_sphere(
    std::vector<glm::vec3>& vertices,
    std::vector<uint32_t>& indices,
    float radius,
    uint sectorCount,
    uint stackCount
);

}  // namespace _geometry
}  // namespace slamd
