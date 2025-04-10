#include <numbers>
#include <slamd/geometry/utils.hpp>

namespace slamd {
namespace _geometry {

size_t generate_sphere(
    std::vector<glm::vec3>& vertices,
    std::vector<uint32_t>& indices,
    float radius,
    uint sectorCount,
    uint stackCount
) {
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    size_t start_idx = vertices.size();

    float pi = std::numbers::pi;
    float sectorStep = 2 * pi / sectorCount;
    float stackStep = pi / stackCount;
    float sectorAngle, stackAngle;

    size_t num_added = 0;

    for (unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = pi / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            vertices.emplace_back(x, y, z);
            num_added++;
        }
    }

    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1 + start_idx);
                indices.push_back(k2 + start_idx);
                indices.push_back(k1 + 1 + start_idx);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1 + start_idx);
                indices.push_back(k2 + start_idx);
                indices.push_back(k2 + 1 + start_idx);
            }
        }
    }

    return num_added;
}

}  // namespace _geometry
}  // namespace slamd