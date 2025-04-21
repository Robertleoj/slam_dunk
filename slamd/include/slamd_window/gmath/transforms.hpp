#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd_window/gmath/angle.hpp>

namespace slamdw {
namespace gmath {

glm::mat4 rot3D(const Angle& angle, glm::vec3 axis);

glm::mat4 rx3D(const Angle& angle);

glm::mat4 ry3D(const Angle& angle);

glm::mat4 rz3D(const Angle& angle);

glm::mat4 tx3D(float amount);
glm::mat4 ty3D(float amount);
glm::mat4 tz3D(float amount);

glm::mat4 t3D(const glm::vec3& amount);

glm::mat4 xy_to_3d(const glm::mat3& mat2d);

glm::mat4 scale_xy(const glm::vec2& scale);
glm::mat4 scale(const glm::vec3& scale);
glm::mat4 scale_all(float scale);

}  // namespace gmath
}  // namespace slamd