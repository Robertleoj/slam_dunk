#pragma once
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mesh.hpp>

namespace slamd {
namespace _geom {

class Box : public Geometry {
   public:
    Box();
    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    Mesh box_mesh;
};

}  // namespace _geom

namespace geom {

using BoxPtr = std::shared_ptr<_geom::Box>;

BoxPtr box();
}  // namespace geom

}  // namespace slamd