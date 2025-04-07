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

glm::mat4 transl(
    const glm::vec3& amount
) {
    glm::mat4 mat(1.0);
    mat[3] = glm::vec4(amount, 1.0);
    return mat;
}

}  // namespace sdunk