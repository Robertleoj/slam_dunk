#include <slamd/geom/poly_line_2d.hpp>
#include <stdexcept>

namespace slamd {
namespace _geom {

PolyLine2D::PolyLine2D(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
) {}

}  // namespace _geom

namespace geom2d {

PolyLinePtr poly_line(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
) {
    return std::make_shared<_geom::PolyLine2D>(points, color, thickness);
}

}  // namespace geom2d
}  // namespace slamd
