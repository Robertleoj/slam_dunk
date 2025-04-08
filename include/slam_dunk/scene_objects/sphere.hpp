#pragma once
#include <glm/glm.hpp>
#include <slam_dunk/scene_object.hpp>
#include <slam_dunk/scene_objects/mono_mesh.hpp>

namespace slamd {

class Sphere : public SceneObject {
   public:
    Sphere(float radius = 1.0f, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0));

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    MonoMesh mesh;
};

}  // namespace slamd