#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/scene_object.hpp>
#include <slamd/scene_objects/arrows.hpp>

namespace slamd {

class Triad : public SceneObject {
   public:
    Triad(float scale = 1.0, float thickness = 0.1);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    Arrows arrows;
};

}  // namespace slamd