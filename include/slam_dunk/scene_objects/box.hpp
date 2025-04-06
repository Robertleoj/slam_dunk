#pragma once
#include <slam_dunk/scene_object.hpp>

namespace sdunk {

class Box : public SceneObject {
    virtual void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};

}  // namespace sdunk