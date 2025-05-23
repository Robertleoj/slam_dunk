#pragma once

#include <glm/glm.hpp>
#include <slamd_window/constants.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mesh.hpp>
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

    static std::shared_ptr<PolyLine> deserialize(
        const slamd::flatb::PolyLine* poly_line_fb
    );

   private:
    std::unique_ptr<Mesh> mesh;
};

}  // namespace _geom

namespace geom {

using PolyLinePtr = std::shared_ptr<_geom::PolyLine>;

PolyLinePtr poly_line(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
);

}  // namespace geom

}  // namespace slamd