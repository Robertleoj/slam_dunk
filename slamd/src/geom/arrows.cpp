#include <slamd/geom/arrows.hpp>
#include <slamd_common/data/mesh.hpp>

namespace slamd {
namespace _geom {

constexpr uint segments = 12;
struct ArrowMesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<uint32_t> indices;
};

Arrows::Arrows(
    const std::vector<glm::vec3>& starts,
    const std::vector<glm::vec3>& ends,
    const std::vector<glm::vec3>& colors,
    float thickness
) {}

}  // namespace _geom

namespace geom {

ArrowsPtr arrows(
    const std::vector<glm::vec3>& starts,
    const std::vector<glm::vec3>& ends,
    const std::vector<glm::vec3>& colors,
    float thickness
) {
    return std::make_shared<_geom::Arrows>(starts, ends, colors, thickness);
}
}  // namespace geom
}  // namespace slamd