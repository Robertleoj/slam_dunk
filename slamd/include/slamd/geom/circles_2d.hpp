#pragma once
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

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    std::optional<gmath::AABB> bounds() override;

   private:
    static MonoInstanced make_mono_instanced(
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii,
        float thickness
    );

    static gmath::AABB make_bounds(
        const std::vector<glm::vec2>& positions,
        const std::vector<float>& radii
    );

    static std::vector<glm::mat4>
    make_transforms(std::vector<glm::vec2> positions, std::vector<float> radii);

   private:
    MonoInstanced circles_instanced;
    gmath::AABB cached_bounds;

    std::vector<glm::vec2> positions;
    std::vector<float> radii;
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