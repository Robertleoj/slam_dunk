#pragma once
#include <memory>
#include <slamd/data/mesh.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mono_instanced.hpp>

namespace slamd {
namespace _geom {

class Circles2D : public Geometry {
   public:
    Circles2D(
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii,
        float thickness
    );

    void update_positions(const std::vector<glm::vec2>& positions);
    void update_colors(const std::vector<glm::vec3>& colors);
    void update_radii(const std::vector<float>& radii);
};

}  // namespace _geom

namespace geom2d {
using CirclesPtr = std::shared_ptr<_geom::Circles2D>;

CirclesPtr circles(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness = 0.1
);
}  // namespace geom2d
}  // namespace slamd