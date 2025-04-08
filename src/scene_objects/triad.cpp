#include <slam_dunk/scene_objects/triad.hpp>

namespace slamd {

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

}  // namespace slamd