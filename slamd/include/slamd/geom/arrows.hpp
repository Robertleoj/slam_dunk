#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/simple_mesh.hpp>

namespace slamd {
namespace _geom {

class Arrows : public Geometry {
   public:
    Arrows(
        const std::vector<glm::vec3>& starts,
        const std::vector<glm::vec3>& ends,
        const std::vector<glm::vec3>& colors,
        float thickness
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    SimpleMesh mesh;
};

}  // namespace _geom

namespace geom {

using _geom::Arrows;

std::shared_ptr<Arrows> arrows(
    const std::vector<glm::vec3>& starts,
    const std::vector<glm::vec3>& ends,
    const std::vector<glm::vec3>& colors,
    float thickness
);

}  // namespace geom
}  // namespace slamd