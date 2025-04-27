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

    flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    ) override;

   private:
    std::vector<glm::vec2> points;
    glm::vec3 color;
    float thickness;
};

}  // namespace _geom

namespace geom2d {
using PolyLinePtr = std::shared_ptr<_geom::PolyLine2D>;

PolyLinePtr poly_line(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
);

}  // namespace geom2d

}  // namespace slamd