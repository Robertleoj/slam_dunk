#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <slam_dunk/scene_object.hpp>
#include <slam_dunk/scene_objects/simple_mesh.hpp>

namespace slamd {

class Box : public SceneObject {
   public:
    Box();
    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    gl::GLuint vao_id;

   private:
    SimpleMesh box_mesh;
};

}  // namespace slamd