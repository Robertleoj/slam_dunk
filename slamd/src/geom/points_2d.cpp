#include <numbers>
#include <ranges>
#include <slamd/geom/points_2d.hpp>

namespace slamd {
namespace _geom {

Points2D::Points2D(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
) {}

}  // namespace _geom
}  // namespace slamd