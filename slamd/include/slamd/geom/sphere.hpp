#pragma once
#include <glm/glm.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mono_mesh.hpp>

namespace slamd {
namespace _geom {

class Sphere : public Geometry {
   public:
    Sphere(float radius, glm::vec3 color);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    MonoMesh mesh;
};

}  // namespace _geom

namespace geom {

using _geom::Sphere;
std::shared_ptr<Sphere>
sphere(float radius = 1.0f, glm::vec3 color = glm::vec3(0.8, 0.2, 0.0));

}  // namespace geom
}  // namespace slamd