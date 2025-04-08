#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slam_dunk/angle.hpp>

namespace slamd {

glm::mat4 rot(const Angle& angle, glm::vec3 axis);

glm::mat4 rx(const Angle& angle);

glm::mat4 ry(const Angle& angle);

glm::mat4 rz(const Angle& angle);

glm::mat4 transl(const glm::vec3& amount);

}  // namespace slamd