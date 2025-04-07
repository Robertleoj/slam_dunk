#pragma once

#include <glm/glm.hpp>

namespace sdunk {

class Camera {
   public:
    Camera(double fov, double near_plane, double far_plane);
    glm::mat4 get_projection_matrix(double aspect_ratio) const;

   private:
    double fov;
    double near_plane;
    double far_plane;
};
}  // namespace sdunk
