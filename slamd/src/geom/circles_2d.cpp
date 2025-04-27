#include <numbers>
#include <ranges>
#include <slamd/geom/circles_2d.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {

namespace _geom {

Circles2D::Circles2D(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
) {}

void Circles2D::update_positions(
    const std::vector<glm::vec2>& positions
) {}

void Circles2D::update_colors(
    const std::vector<glm::vec3>& colors
) {}

void Circles2D::update_radii(
    const std::vector<float>& radii
) {}

}  // namespace _geom

namespace geom2d {

CirclesPtr circles(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
) {
    return std::make_shared<_geom::Circles2D>(
        positions,
        colors,
        radii,
        thickness
    );
}
}  // namespace geom2d
}  // namespace slamd