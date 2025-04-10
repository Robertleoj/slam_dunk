#include <slamd/geom/triad.hpp>

namespace slamd {
namespace _geom {

Arrows make_arrows(
    float scale,
    float thickness
) {
    glm::vec3 origin(0.0, 0.0, 0.0);

    return Arrows(
        {origin, origin, origin},
        {{scale, 0.0, 0.0}, {0.0, scale, 0.0}, {0.0, 0.0, scale}},
        {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
        thickness
    );
}

Triad::Triad(
    float scale,
    float thickness
)
    : arrows(make_arrows(scale, thickness)) {}

void Triad::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->arrows.render(model, view, projection);
}

}  // namespace _geom

namespace geom {
std::shared_ptr<Triad> triad(
    float scale,
    float thickness
) {
    return std::make_shared<Triad>(scale, thickness);
}
}  // namespace geom

}  // namespace slamd