#include <slamd/gmath/transforms.hpp>

namespace slamd {
namespace gmath {

glm::mat4 rot3D(
    const Angle& angle,
    glm::vec3 axis
) {
    return glm::rotate(glm::mat4(1.0), angle.rad(), axis);
}

glm::mat4 rx3D(
    const Angle& angle
) {
    return rot3D(angle, glm::vec3(1.0, 0.0, 0.0));
}

glm::mat4 ry3D(
    const Angle& angle
) {
    return rot3D(angle, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 rz3D(
    const Angle& angle

) {
    return rot3D(angle, glm::vec3(0.0, 0.0, 1.0));
}

glm::mat4 t3D(
    const glm::vec3& amount
) {
    glm::mat4 mat(1.0);
    mat[3] = glm::vec4(amount, 1.0);
    return mat;
}

glm::mat4 tx3D(
    float amount
) {
    return t3D(glm::vec3(amount, 0.0f, 0.0f));
}

glm::mat4 ty3D(
    float amount
) {
    return t3D(glm::vec3(0.0f, amount, 0.0f));
}

glm::mat4 tz3D(
    float amount
) {
    return t3D(glm::vec3(0.0f, 0.0f, amount));
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
    const glm::vec2& scale_vec
) {
    return scale(glm::vec3(scale_vec, 1.0));
}

glm::mat4 scale(
    const glm::vec3& scale
) {
    return glm::scale(glm::mat4(1.0f), scale);
}

}  // namespace gmath
}  // namespace slamd