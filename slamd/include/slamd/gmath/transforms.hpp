#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/gmath/angle.hpp>

namespace slamd {
namespace gmath {

glm::mat4 rot(const Angle& angle, glm::vec3 axis);

glm::mat4 rx(const Angle& angle);

glm::mat4 ry(const Angle& angle);

glm::mat4 rz(const Angle& angle);

glm::mat4 transl(const glm::vec3& amount);

glm::mat4 xy_to_3d(const glm::mat3& mat2d);

glm::mat4 scale_xy(const glm::vec2& scale);

}  // namespace gmath
}  // namespace slamd