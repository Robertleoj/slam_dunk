#pragma once
#include <glm/glm.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mesh.hpp>

namespace slamdw {
namespace _geom {

class Sphere : public Geometry {
   public:
    Sphere(float radius, glm::vec3 color);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    Mesh mesh;
};

}  // namespace _geom

namespace geom {

using SpherePtr = std::shared_ptr<_geom::Sphere>;

SpherePtr
sphere(float radius = 1.0f, glm::vec3 color = glm::vec3(0.8, 0.2, 0.0));

}  // namespace geom
}  // namespace slamd