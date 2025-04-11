#include <glm/glm.hpp>
#include <numbers>
#include <ranges>
#include <slamd/geometry/sphere.hpp>
#include <slamd/geometry/utils.hpp>
#include <vector>

namespace slamd {
namespace _geometry {

MonoMesh make_sphere_mesh(
    float radius,
    glm::vec3 color
) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;

    generate_sphere(vertices, indices, normals, radius, 20, 20);

    data::Mesh mesh_data;

    mesh_data.triangle_indices = std::move(indices);
    for (const auto& [vert, norm] : std::views::zip(vertices, normals)) {
        mesh_data.vertices.emplace_back(vert, norm);
    }

    return MonoMesh(mesh_data, color);
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

}  // namespace _geometry

namespace geometry {
std::shared_ptr<Sphere> sphere(
    float radius,
    glm::vec3 color
) {
    return std::make_shared<Sphere>(radius, color);
}
}  // namespace geometry

}  // namespace slamd