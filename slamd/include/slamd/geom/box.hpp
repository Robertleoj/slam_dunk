#pragma once
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/simple_mesh.hpp>

namespace slamd {
namespace _geom {

class Box : public Geometry {
   public:
    Box();
    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    SimpleMesh box_mesh;
};

}  // namespace _geom

namespace geom {

using _geom::Box;

std::shared_ptr<Box> box();
}  // namespace geom

}  // namespace slamd