#pragma once
#include <glm/glm.hpp>
#include <slamd_common/gmath/aabb.hpp>

namespace slamd {
namespace gmath {

class Rect2D {
   public:
    glm::vec2 top_left;
    glm::vec2 bottom_right;

   public:
    Rect2D(const glm::vec2& top_left, const glm::vec2& bottom_right);
    Rect2D(const Rect2D& other);
    Rect2D& operator=(const Rect2D&) = default;

    Rect2D translate(glm::vec2 amount) const;

    static Rect2D from_aabb3d(const AABB& aabb3d);
    static Rect2D
    from_center_size(const glm::vec2& center, const glm::vec2& size);
    static Rect2D center_cover(const Rect2D& to_cover, float required_aspect);

    glm::vec2 unnormalize(const glm::vec2& normalized_center) const;

    glm::vec2 size() const;

    glm::vec2 center() const;
};

}  // namespace gmath

}  // namespace slamd