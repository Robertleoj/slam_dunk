#pragma once

#include <glm/glm.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/geometry/mono_mesh.hpp>
#include <slamd/constants.hpp>
#include <vector>

namespace slamd {
namespace _geometry {

class PolyLine : public Geometry {
   public:
    PolyLine(
        const std::vector<glm::vec3>& points,
        float thickness,
        const glm::vec3& color,
        float min_brightness = _const::default_min_brightness
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    MonoMesh mesh;
};

}  // namespace _geometry

namespace geometry {

using _geometry::PolyLine;
std::shared_ptr<PolyLine> poly_line(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
);

}  // namespace geometry

}  // namespace slamd