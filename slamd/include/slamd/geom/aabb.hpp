#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace slamd {
namespace _geom {

class AABB {
   public:
    const glm::vec3 min;
    const glm::vec3 max;

   public:
    AABB(const glm::vec3& min, const glm::vec3& max);
    AABB();
    AABB transform(const glm::mat4& transform);

    static std::optional<AABB> combine(const std::vector<AABB>& bounds);
};

}  // namespace _geom
}  // namespace slamd