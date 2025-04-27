#include <numbers>
#include <ranges>
#include <slamd_common/utils/mesh.hpp>

namespace slamd {
namespace _utils {

size_t generate_sphere(
    std::vector<glm::vec3>& vertices,
    std::vector<uint32_t>& indices,
    std::vector<glm::vec3>& normals,  // ⬅️ Add this
    float radius,
    uint sectorCount,
    uint stackCount
) {
    float x, y, z, xy;
    float pi = std::numbers::pi;
    float sectorStep = 2 * pi / sectorCount;
    float stackStep = pi / stackCount;
    float sectorAngle, stackAngle;

    size_t start_idx = vertices.size();
    size_t num_added = 0;

    for (uint i = 0; i <= stackCount; ++i) {
        stackAngle = pi / 2 - i * stackStep;  // from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);       // r * cos(u)
        z = radius * sinf(stackAngle);        // r * sin(u)

        for (uint j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);  // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);  // r * cos(u) * sin(v)

            glm::vec3 pos(x, y, z);
            vertices.push_back(pos);
            normals.push_back(
                glm::normalize(pos)
            );  // 💪 Normals = normalized pos

            ++num_added;
        }
    }

    uint k1, k2;
    for (uint i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (uint j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(start_idx + k1);
                indices.push_back(start_idx + k2);
                indices.push_back(start_idx + k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(start_idx + k1 + 1);
                indices.push_back(start_idx + k2);
                indices.push_back(start_idx + k2 + 1);
            }
        }
    }

    return num_added;
}

std::vector<glm::vec3> compute_vertex_normals(
    const std::vector<glm::vec3>& vertices,
    const std::vector<uint32_t>& triangle_indices
) {
    std::vector<glm::vec3> normals;

    // compute normals
    normals.resize(vertices.size(), glm::vec3(0.0f));

    for (size_t i = 0; i < triangle_indices.size(); i += 3) {
        uint32_t i0 = triangle_indices[i];
        uint32_t i1 = triangle_indices[i + 1];
        uint32_t i2 = triangle_indices[i + 2];

        glm::vec3 v0 = vertices[i0];
        glm::vec3 v1 = vertices[i1];
        glm::vec3 v2 = vertices[i2];

        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }

    for (auto& n : normals) {
        n = glm::normalize(n);
    }
    return normals;
}

}  // namespace _utils
}  // namespace slamd