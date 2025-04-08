#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <slamd/geometry/geometry.hpp>
#include <slamd/geometry/simple_mesh.hpp>

namespace slamd {
namespace geometry {

class Box : public Geometry {
   public:
    Box();
    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    gl::GLuint vao_id;

   private:
    SimpleMesh box_mesh;
};

std::shared_ptr<Box> box();

}  // namespace geometry
}  // namespace slamd