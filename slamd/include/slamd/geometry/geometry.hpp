#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <slamd/geom/aabb.hpp>

namespace slamd {
namespace _geometry {

class Geometry {
   public:
    virtual void
    render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) = 0;

    virtual ~Geometry() = default;

    virtual std::optional<_geom::AABB> bounds();
};
}  // namespace _geometry
}  // namespace slamd