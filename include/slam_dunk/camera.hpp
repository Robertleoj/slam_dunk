#pragma once

#include <glm/glm.hpp>

namespace sdunk {

class Camera {
   public:
    Camera(double fov, double near_plane, double far_plane);
    glm::mat4 get_projection_matrix(double aspect_ratio) const;
    glm::mat4 get_view_matrix() const;

   public:
    glm::mat4 pose;

   private:
    double fov;
    double near_plane;
    double far_plane;
};
}  // namespace sdunk
