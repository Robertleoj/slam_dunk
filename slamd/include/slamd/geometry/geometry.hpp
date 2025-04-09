#pragma once
#include <glm/glm.hpp>

namespace slamd {
namespace geometry {

class Geometry {
   public:
    virtual void
    render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) = 0;

    virtual ~Geometry() = default;
};
}  // namespace geometry
}  // namespace slamd