#pragma once
#include <memory>
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mono_instanced.hpp>
#include <slamd_common/data/mesh.hpp>

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

    flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    ) override;

    void update_positions(const std::vector<glm::vec2>& positions);
    void update_colors(const std::vector<glm::vec3>& colors);
    void update_radii(const std::vector<float>& radii);

   private:
    std::vector<glm::vec2> positions;
    std::vector<glm::vec3> colors;
    std::vector<float> radii;
    float thickness;
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