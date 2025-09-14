#include <slamd_common/gmath/misc.hpp>

namespace slamd {
namespace gmath {
glm::vec3 get_orthogonal_vector(
    const glm::vec3& vec
) {
    auto normalized = glm::normalize(vec);
    glm::vec3 to_cross(1, 0, 0);

    if (glm::abs(glm::dot(normalized, to_cross)) < 1e-5) {
        to_cross = glm::vec3(0, 1, 0);
    }

    auto cross = glm::cross(normalized, to_cross);
    return glm::normalize(cross);
}

glm::mat4 make_frame(
    const glm::vec3& x_axis,
    const glm::vec3& y_axis,
    const glm::vec3& z_axis,
    const glm::vec3& translation
) {
    glm::mat4 frame(1.0f);

    frame[0] = glm::vec4(x_axis, 0.0f);
    frame[1] = glm::vec4(y_axis, 0.0f);
    frame[2] = glm::vec4(z_axis, 0.0f);
    frame[3] = glm::vec4(translation, 1.0f);

    return frame;
}

}  // namespace gmath

}  // namespace slamd
