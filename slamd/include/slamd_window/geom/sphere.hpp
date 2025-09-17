#pragma once
#include <glm/glm.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mesh.hpp>

namespace slamd {
namespace _geom {

class Sphere : public Geometry {
   public:
    Sphere(float radius, glm::vec3 color);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    static std::shared_ptr<Sphere> deserialize(
        const slamd::flatb::Sphere* sphere_fb
    );

   private:
    std::unique_ptr<Mesh> mesh;
};

}  // namespace _geom
}  // namespace slamd