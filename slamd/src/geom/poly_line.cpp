#include <glm/gtc/matrix_transform.hpp>
#include <numbers>
#include <slamd/geom/poly_line.hpp>
#include <slamd/geom/utils.hpp>

namespace slamd {
namespace _geom {

PolyLine::PolyLine(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color,
    float min_brightness
) {}

}  // namespace _geom

namespace geom {

PolyLinePtr poly_line(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
) {
    return std::make_shared<_geom::PolyLine>(points, thickness, color);
}
}  // namespace geom
}  // namespace slamd