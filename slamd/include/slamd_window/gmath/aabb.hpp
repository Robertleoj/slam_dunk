#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace slamdw {
namespace gmath {

class AABB {
   public:
    glm::vec3 min;
    glm::vec3 max;

   public:
    AABB(const glm::vec3& min, const glm::vec3& max);
    AABB();

    static AABB around_points(const std::vector<glm::vec3>& points);
    static AABB around_points(const std::vector<glm::vec2>& points);

    AABB transform(const glm::mat4& transform);

    static std::optional<AABB> combine(const std::vector<AABB>& bounds);
};

}  // namespace gmath
}  // namespace slamd