#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/scene_object.hpp>
#include <slamd/scene_objects/simple_mesh.hpp>

namespace slamd {

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

}  // namespace slamd