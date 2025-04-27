#include <glm/glm.hpp>
#include <numbers>
#include <ranges>
#include <slamd_common/utils/mesh.hpp>
#include <slamd_window/geom/sphere.hpp>
#include <vector>

namespace slamdw {
namespace _geom {

Mesh make_sphere_mesh(
    float radius,
    glm::vec3 color
) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;

    slamd::_utils::generate_sphere(vertices, indices, normals, radius, 20, 20);

    auto mesh_data = slamd::data::MeshDataBuilder()
                         .set_positions(std::move(vertices))
                         .set_indices(std::move(indices))
                         .set_normals(std::move(normals))
                         .set_colors(color)
                         .build();

    return Mesh(std::move(mesh_data));
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

}  // namespace _geom

namespace geom {

SpherePtr sphere(
    float radius,
    glm::vec3 color
) {
    return std::make_shared<_geom::Sphere>(radius, color);
}
}  // namespace geom

}  // namespace slamdw