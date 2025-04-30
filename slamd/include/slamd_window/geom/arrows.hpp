#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mesh.hpp>

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

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    static std::shared_ptr<Arrows> deserialize(
        const slamd::flatb::Arrows* arrows_fb
    );

   private:
    Mesh mesh;
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