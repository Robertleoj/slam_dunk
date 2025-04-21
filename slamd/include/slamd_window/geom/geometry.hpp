#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <slamd/gmath/aabb.hpp>

namespace slamd {
namespace _geom {

class Geometry {
   public:
    virtual void
    render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) = 0;

    virtual ~Geometry() = default;

    virtual std::optional<gmath::AABB> bounds();
};

}  // namespace _geom
}  // namespace slamd