#pragma once

#include <glm/glm.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mesh.hpp>

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

    std::optional<gmath::AABB> bounds() override;

   private:
    static Mesh make_mesh(
        const std::vector<glm::vec2>& points,
        const glm::vec3& color,
        float thickness
    );

   private:
    Mesh line_mesh;
    gmath::AABB cached_bounds;
};

}  // namespace _geom

namespace geom2d {
using _geom::PolyLine2D;

std::shared_ptr<PolyLine2D> poly_line(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
);

}  // namespace geom2d

}  // namespace slamd