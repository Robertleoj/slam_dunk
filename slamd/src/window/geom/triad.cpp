#include <slamd_common/gmath/transforms.hpp>
#include <slamd_window/geom/triad.hpp>

namespace slamd {
namespace _geom {

std::shared_ptr<Triad> Triad::deserialize(
    const slamd::flatb::Triad* triad_fb
) {
    return std::make_shared<Triad>(triad_fb->scale(), triad_fb->thickness());
}

Arrows make_arrows(
    float thickness
) {
    glm::vec3 origin(0.0, 0.0, 0.0);

    return Arrows(
        {origin, origin, origin},
        {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
        {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
        thickness
    );
}

Triad::Triad(
    float scale,
    float thickness
)
    : arrows(make_arrows(thickness)) {
    this->scale_transform = slamd::gmath::scale(glm::vec3(scale, scale, scale));
}

void Triad::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->arrows.render(model * this->scale_transform, view, projection);
}

}  // namespace _geom

namespace geom {

TriadPtr triad(
    float scale,
    float thickness
) {
    return std::make_shared<_geom::Triad>(scale, thickness);
}
}  // namespace geom

}  // namespace slamd