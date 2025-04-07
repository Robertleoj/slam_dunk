#include <glm/glm.hpp>
#include <numbers>
#include <slam_dunk/scene_objects/sphere.hpp>
#include <vector>

namespace sdunk {

void generate_sphere(
    std::vector<glm::vec3>& vertices,
    std::vector<uint32_t>& indices,
    float radius,
    uint sectorCount,
    uint stackCount
) {
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float pi = std::numbers::pi;
    float sectorStep = 2 * pi / sectorCount;
    float stackStep = pi / stackCount;
    float sectorAngle, stackAngle;

    for (unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = pi / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            vertices.emplace_back(x, y, z);
        }
    }

    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

MonoMesh make_sphere_mesh(
    float radius,
    glm::vec3 color
) {
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;

    generate_sphere(vertices, indices, radius, 20, 20);

    return MonoMesh(vertices, indices, color);
}

Sphere::Sphere(
    float radius,
    glm::vec3 color
)
    : mesh(make_sphere_mesh(radius, color)) {}

void Sphere::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->mesh.render(model, view, projection);
};

}  // namespace sdunk