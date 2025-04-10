#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace slamd {
namespace _geom {

class AABB {
   public:
    glm::vec3 min;
    glm::vec3 max;

   public:
    AABB(const glm::vec3& min, const glm::vec3& max);
    AABB();
    AABB transform(const glm::mat4& transform);

    static std::optional<AABB> combine(const std::vector<AABB>& bounds);
};

class AABB2D {
   public:
    glm::vec2 min;
    glm::vec2 max;

   public:
    AABB2D(const glm::vec2& min, const glm::vec2& max);
    AABB2D(const AABB2D& other);

    AABB2D translate(glm::vec2 amount) const;

    static AABB2D from_3d(const AABB& aabb3d);
    static AABB2D center_cover(const AABB2D& to_cover, float required_aspect);

    glm::vec2 size() const;
};

}  // namespace _geom
}  // namespace slamd