#pragma once

#include <glm/glm.hpp>

namespace slamd {
namespace data {

struct ColoredVertex {
    glm::vec3 position;
    glm::vec3 color;
};

struct ColoredMesh {
    std::vector<ColoredVertex> vertices;
    std::vector<uint32_t> triangle_indices;
};

}  // namespace data
}  // namespace slamd