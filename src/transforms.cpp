#include <slam_dunk/transforms.hpp>

namespace sdunk {
glm::mat4 rot(
    const Angle& angle,
    glm::vec3 axis
) {
    return glm::rotate(glm::mat4(1.0), angle.rad(), axis);
}

glm::mat4 rx(
    const Angle& angle
) {
    return rot(angle, glm::vec3(1.0, 0.0, 0.0));
}

glm::mat4 ry(
    const Angle& angle
) {
    return rot(angle, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 rz(
    const Angle& angle

) {
    return rot(angle, glm::vec3(0.0, 0.0, 1.0));
}

}  // namespace sdunk