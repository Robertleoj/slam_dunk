#include <slamd/geom/aabb.hpp>

namespace slamd {
namespace _geom {

AABB::AABB(
    const glm::vec3& min,
    const glm::vec3& max
)
    : min(min),
      max(max) {}

AABB::AABB()
    : AABB(glm::vec3(0.0f), glm::vec3(0.0f)) {}

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

AABB2D::AABB2D(
    const glm::vec2& min,
    const glm::vec2& max
)
    : min(min),
      max(max) {}

AABB2D AABB2D::from_3d(
    const AABB& aabb3d
) {
    return AABB2D(glm::vec2(aabb3d.min), glm::vec2(aabb3d.max));
}

AABB2D AABB2D::center_cover(
    const AABB2D& to_cover,
    float required_aspect
) {
    glm::vec2 center = 0.5f * (to_cover.min + to_cover.max);
    glm::vec2 size = to_cover.max - to_cover.min;
    float current_aspect = size.x / size.y;

    glm::vec2 new_size = size;

    if (current_aspect < required_aspect) {
        // Need to widen
        new_size.x = size.y * required_aspect;
    } else {
        // Need to heighten
        new_size.y = size.x / required_aspect;
    }

    glm::vec2 new_min = center - 0.5f * new_size;
    glm::vec2 new_max = center + 0.5f * new_size;

    return AABB2D(new_min, new_max);
}

AABB2D::AABB2D(
    const AABB2D& other
)
    : min(other.min),
      max(other.max) {}

glm::vec2 AABB2D::size() const {
    return this->max - this->min;
}

AABB2D AABB2D::translate(
    glm::vec2 amount
) const {
    return AABB2D(this->min + amount, this->max + amount);
}

}  // namespace _geom
}  // namespace slamd