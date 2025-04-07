#pragma once

#include <glm/glm.hpp>
#include <slam_dunk/angle.hpp>

namespace sdunk {

class Arcball {
   public:
    glm::mat4 center;
    Angle theta;
    Angle phi;
    float radius;

   public:
    Arcball();
    void rotate(Angle x, Angle y);
    glm::mat4 camera_in_center() const;
    glm::mat4 view_matrix() const;
};

}  // namespace sdunk