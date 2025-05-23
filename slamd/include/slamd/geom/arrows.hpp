#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mesh.hpp>

namespace slamd {
namespace _geom {

/**
 * TOOD: use MonoInstanced to allow modifying arrows
 */
class Arrows : public Geometry {
   public:
    Arrows(
        const std::vector<glm::vec3>& starts,
        const std::vector<glm::vec3>& ends,
        const std::vector<glm::vec3>& colors,
        float thickness
    );

    flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    ) override;

   private:
    std::vector<glm::vec3> starts;
    std::vector<glm::vec3> ends;
    std::vector<glm::vec3> colors;
    float thickness;
};

}  // namespace _geom

namespace geom {

using ArrowsPtr = std::shared_ptr<_geom::Arrows>;

ArrowsPtr arrows(
    const std::vector<glm::vec3>& starts,
    const std::vector<glm::vec3>& ends,
    const std::vector<glm::vec3>& colors,
    float thickness
);

}  // namespace geom
}  // namespace slamd