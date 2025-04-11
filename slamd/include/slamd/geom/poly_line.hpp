#pragma once

#include <glm/glm.hpp>
#include <slamd/constants.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mono_mesh.hpp>
#include <vector>

namespace slamd {
namespace _geom {

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

}  // namespace _geom

namespace geom {

using _geom::PolyLine;
std::shared_ptr<PolyLine> poly_line(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
);

}  // namespace geom

}  // namespace slamd