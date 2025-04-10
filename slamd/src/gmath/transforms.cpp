#include <slamd/gmath/transforms.hpp>

namespace slamd {
namespace gmath {
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

glm::mat4 xy_to_3d(
    const glm::mat3& mat2d
) {
    glm::mat4 mat3d(1.0f);  // identity mat4
    mat3d[0][0] = mat2d[0][0];
    mat3d[0][1] = mat2d[0][1];
    mat3d[1][0] = mat2d[1][0];
    mat3d[1][1] = mat2d[1][1];
    mat3d[3][0] = mat2d[2][0];
    mat3d[3][1] = mat2d[2][1];
    return mat3d;
}

glm::mat4 scale_xy(
    const glm::vec2& scale
) {
    return glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0));
}

}  // namespace gmath
}  // namespace slamd