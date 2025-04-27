#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <slamd_common/gmath/aabb.hpp>

namespace slamd {
namespace _geom {

class Geometry {
   public:
    virtual ~Geometry() = default;
};

}  // namespace _geom
}  // namespace slamd