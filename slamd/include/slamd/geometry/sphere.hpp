#pragma once
#include <glm/glm.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/geometry/mono_mesh.hpp>

namespace slamd {
namespace geometry {

class Sphere : public Geometry {
   public:
    Sphere(float radius, glm::vec3 color);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    MonoMesh mesh;
};

std::shared_ptr<Sphere>
sphere(float radius = 1.0f, glm::vec3 color = glm::vec3(0.8, 0.2, 0.0));

}  // namespace geometry
}  // namespace slamd