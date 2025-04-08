#pragma once

#include <glm/glm.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/geometry/mono_mesh.hpp>
#include <vector>

namespace slamd {
namespace geometry {

class PolyLine : public Geometry {
   public:
    PolyLine(
        const std::vector<glm::vec3>& points,
        float thickness,
        const glm::vec3& color
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    MonoMesh mesh;
};

std::shared_ptr<PolyLine> poly_line(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
);

}  // namespace geometry
}  // namespace slamd