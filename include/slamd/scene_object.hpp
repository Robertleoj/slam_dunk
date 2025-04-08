#pragma once
#include <glm/glm.hpp>

namespace slamd {

class SceneObject {
   public:
    virtual void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    virtual ~SceneObject() = default;
};
}  // namespace slamd