#pragma once

#include <glm/glm.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mesh.hpp>

namespace slamd {
namespace _geom {

class PolyLine2D : public Geometry {
   public:
    PolyLine2D(
        const std::vector<glm::vec2>& points,
        const glm::vec3& color,
        float thickness
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    static std::shared_ptr<PolyLine2D> deserialize(
        const slamd::flatb::PolyLine2D* poly_line_fb
    );

    std::optional<slamd::gmath::AABB> bounds() override;

   private:
    static std::unique_ptr<Mesh> make_mesh(
        const std::vector<glm::vec2>& points,
        const glm::vec3& color,
        float thickness
    );

   private:
    std::unique_ptr<Mesh> line_mesh;
    slamd::gmath::AABB cached_bounds;
};

}  // namespace _geom
}  // namespace slamd