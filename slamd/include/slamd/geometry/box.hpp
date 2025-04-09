#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <slamd/geometry/geometry.hpp>
#include <slamd/geometry/simple_mesh.hpp>
#include <slamd/thread_box.hpp>

namespace slamd {
namespace _geometry {

class Box : public Geometry {
   public:
    Box();
    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    SimpleMesh box_mesh;
};

}  // namespace _geometry

namespace geometry {
    
using _geometry::Box;

std::shared_ptr<Box> box();
}  // namespace geometry

}  // namespace slamd