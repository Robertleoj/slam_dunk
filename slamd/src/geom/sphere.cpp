#include <glm/glm.hpp>
#include <numbers>
#include <ranges>
#include <slamd/geom/sphere.hpp>
#include <slamd_common/data/mesh.hpp>
#include <vector>

namespace slamd {
namespace _geom {

Sphere::Sphere(
    float radius,
    glm::vec3 color
) {}

}  // namespace _geom

namespace geom {

SpherePtr sphere(
    float radius,
    glm::vec3 color
) {
    return std::make_shared<_geom::Sphere>(radius, color);
}
}  // namespace geom

}  // namespace slamd