#include <numbers>
#include <ranges>
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

data::ColoredMesh make_colored_mesh(
    const std::vector<glm::vec3>& vertex_positions,
    const std::vector<glm::vec3>& vertex_colors,
    const std::vector<uint32_t>& triangle_indices
) {
    auto normals = compute_vertex_normals(vertex_positions, triangle_indices);

    data::ColoredMesh mesh;

    for (const auto& [vert, col, norm] :
         std::views::zip(vertex_positions, vertex_colors, normals)) {
        mesh.vertices.emplace_back(vert, col, norm);
    }

    mesh.triangle_indices = std::move(triangle_indices);

    return mesh;
}

data::Mesh make_mesh(
    const std::vector<glm::vec3>& vertex_positions,
    const std::vector<uint32_t>& triangle_indices
) {
    auto normals = compute_vertex_normals(vertex_positions, triangle_indices);

    data::Mesh mesh;

    for (const auto& [vert, norm] :
         std::views::zip(vertex_positions, normals)) {
        mesh.vertices.emplace_back(vert, norm);
    }

    mesh.triangle_indices = std::move(triangle_indices);

    return mesh;
}

}  // namespace _geometry
}  // namespace slamd