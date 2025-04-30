#pragma once
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mesh.hpp>

namespace slamd {
namespace _geom {

class Box : public Geometry {
   public:
    Box();
    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    static std::shared_ptr<Box> deserialize(const slamd::flatb::Box* box_fb);

   private:
    Mesh box_mesh;
};

}  // namespace _geom

namespace geom {

using BoxPtr = std::shared_ptr<_geom::Box>;

BoxPtr box();
}  // namespace geom

}  // namespace slamd