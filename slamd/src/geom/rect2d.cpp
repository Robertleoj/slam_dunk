#include <spdlog/spdlog.h>
#include <slamd/geom/rect2d.hpp>

namespace slamd {
namespace _geom {

Rect2D::Rect2D(
    const glm::vec2& top_left,
    const glm::vec2& bottom_right
)
    : top_left(top_left),
      bottom_right(bottom_right) {}

Rect2D Rect2D::from_aabb3d(
    const AABB& aabb3d
) {
    glm::vec2 top_left(aabb3d.min.x, aabb3d.min.y);
    glm::vec2 bottom_right(aabb3d.max.x, aabb3d.max.y);

    return Rect2D(top_left, bottom_right);
}

Rect2D Rect2D::center_cover(
    const Rect2D& to_cover,
    float required_aspect
) {
    glm::vec2 center = to_cover.center();
    glm::vec2 size = to_cover.size();

    float current_aspect = size.x / size.y;

    glm::vec2 new_size = size;

    if (current_aspect < required_aspect) {
        // Need to widen
        new_size.x = size.y * required_aspect;
    } else {
        // Need to heighten
        new_size.y = size.x / required_aspect;
    }

    return Rect2D::from_center_size(center, new_size);
}

Rect2D::Rect2D(
    const Rect2D& other
)
    : top_left(other.top_left),
      bottom_right(other.bottom_right) {}

glm::vec2 Rect2D::size() const {
    return this->bottom_right - this->top_left;
}

Rect2D Rect2D::translate(
    glm::vec2 amount
) const {
    spdlog::debug("Translating by {} {}", amount.x, amount.y);
    return Rect2D(this->top_left + amount, this->bottom_right + amount);
}

glm::vec2 Rect2D::center() const {
    return (this->top_left + this->bottom_right) / 2.0f;
}

Rect2D Rect2D::from_center_size(
    const glm::vec2& center,
    const glm::vec2& size
) {
    glm::vec2 top_left = center - size / 2.0f;
    glm::vec2 bottom_right = center + size / 2.0f;

    return Rect2D(top_left, bottom_right);
}

glm::vec2 Rect2D::unnormalize(
    const glm::vec2& normalized_center
) const {
    spdlog::debug("Rect size: {} {}", this->size().x, this->size().y);
    glm::vec2 center = (normalized_center * this->size()) + this->top_left;

    return center;
}

}  // namespace _geom
}  // namespace slamd