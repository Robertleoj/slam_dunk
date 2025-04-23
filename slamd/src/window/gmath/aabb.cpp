#include <slamd_common/gmath/aabb.hpp>
#include <stdexcept>

namespace slamdw {
namespace gmath {

AABB::AABB(
    const glm::vec3& min,
    const glm::vec3& max
)
    : min(min),
      max(max) {}

AABB::AABB()
    : AABB(glm::vec3(0.0f), glm::vec3(0.0f)) {}

AABB AABB::around_points(
    const std::vector<glm::vec3>& points
) {
    if (points.empty()) {
        throw std::invalid_argument("Cannot make bounds around no points");
    }
    auto min = points[0];
    auto max = points[1];

    for (const auto& pt : points) {
        min.x = glm::min(min.x, pt.x);
        min.y = glm::min(min.y, pt.y);
        min.z = glm::min(min.z, pt.z);

        max.x = glm::max(max.x, pt.x);
        max.y = glm::max(max.y, pt.y);
        max.z = glm::max(max.z, pt.z);
    }

    return AABB(min, max);
}

AABB AABB::around_points(
    const std::vector<glm::vec2>& points
) {
    if (points.empty()) {
        throw std::invalid_argument("Cannot make bounds around no points");
    }
    auto min = points[0];
    auto max = points[1];

    for (const auto& pt : points) {
        min.x = glm::min(min.x, pt.x);
        min.y = glm::min(min.y, pt.y);

        max.x = glm::max(max.x, pt.x);
        max.y = glm::max(max.y, pt.y);
    }

    return AABB(glm::vec3(min, 0.0f), glm::vec3(max, 0.0f));
}

AABB AABB::transform(
    const glm::mat4& transform
) {
    glm::vec3 corners[8];
    glm::vec3 min = this->min;
    glm::vec3 max = this->max;

    corners[0] = glm::vec3(transform * glm::vec4(min.x, min.y, min.z, 1.0f));
    corners[1] = glm::vec3(transform * glm::vec4(max.x, min.y, min.z, 1.0f));
    corners[2] = glm::vec3(transform * glm::vec4(min.x, max.y, min.z, 1.0f));
    corners[3] = glm::vec3(transform * glm::vec4(max.x, max.y, min.z, 1.0f));
    corners[4] = glm::vec3(transform * glm::vec4(min.x, min.y, max.z, 1.0f));
    corners[5] = glm::vec3(transform * glm::vec4(max.x, min.y, max.z, 1.0f));
    corners[6] = glm::vec3(transform * glm::vec4(min.x, max.y, max.z, 1.0f));
    corners[7] = glm::vec3(transform * glm::vec4(max.x, max.y, max.z, 1.0f));

    glm::vec3 new_min = corners[0];
    glm::vec3 new_max = corners[0];
    for (int i = 1; i < 8; ++i) {
        new_min = glm::min(new_min, corners[i]);
        new_max = glm::max(new_max, corners[i]);
    }
    return AABB(new_min, new_max);
}

std::optional<AABB> AABB::combine(
    const std::vector<AABB>& boxes
) {
    if (boxes.empty()) {
        return std::nullopt;
    }

    glm::vec3 combined_min = boxes[0].min;
    glm::vec3 combined_max = boxes[0].max;

    for (size_t i = 1; i < boxes.size(); ++i) {
        combined_min = glm::min(combined_min, boxes[i].min);
        combined_max = glm::max(combined_max, boxes[i].max);
    }

    return AABB(combined_min, combined_max);
}

}  // namespace gmath
}  // namespace slamd