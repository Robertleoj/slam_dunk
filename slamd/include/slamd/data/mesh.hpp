#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace slamd {
namespace data {

struct ColoredVertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct ColoredMesh {
    std::vector<ColoredVertex> vertices;
    std::vector<uint32_t> triangle_indices;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> triangle_indices;
};

}  // namespace data
}  // namespace slamd