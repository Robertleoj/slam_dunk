#pragma once
#include <glm/glm.hpp>

namespace slamd {
namespace geometry2d {

class Geometry2D {
   public:
    virtual void render(glm::mat3 model, glm::mat3 view, glm::mat3 projection);

    virtual ~Geometry2D() = default;
};
}  // namespace geometry2d
}  // namespace slamd