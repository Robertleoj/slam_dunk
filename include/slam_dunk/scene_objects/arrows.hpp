#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slam_dunk/scene_object.hpp>
#include <slam_dunk/scene_objects/simple_mesh.hpp>

namespace sdunk {

class Arrows : public SceneObject {
   public:
    Arrows(
        const std::vector<glm::vec3>& starts,
        const std::vector<glm::vec3>& ends,
        const std::vector<glm::vec3>& colors,
        float thickness
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    SimpleMesh mesh;
};

}  // namespace sdunk