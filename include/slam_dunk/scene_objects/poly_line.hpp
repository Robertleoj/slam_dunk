#pragma once

#include <glm/glm.hpp>
#include <slam_dunk/scene_object.hpp>
#include <slam_dunk/scene_objects/mono_mesh.hpp>
#include <vector>

namespace slamd {

class PolyLine : public SceneObject {
   public:
    PolyLine(
        const std::vector<glm::vec3>& points,
        float thickness,
        const glm::vec3& color
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    MonoMesh mesh;
};

}  // namespace slamd