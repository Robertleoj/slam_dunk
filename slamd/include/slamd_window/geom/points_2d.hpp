#pragma once

#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mesh.hpp>

namespace slamd {
namespace _geom {

class Points2D : public Geometry {
   public:
    Points2D(
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    std::optional<slamd::gmath::AABB> bounds() override;

    static std::shared_ptr<Points2D> deserialize(
        const slamd::flatb::Points2D* points2d_fb
    );

   private:
    std::unique_ptr<Mesh> mesh;
    slamd::gmath::AABB cached_bounds;
};

}  // namespace _geom
}  // namespace slamd