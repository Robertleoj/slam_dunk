#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/geometry/arrows.hpp>
#include <slamd/geometry/geometry.hpp>

namespace slamd {
namespace _geometry {

class Triad : public Geometry {
   public:
    Triad(float scale, float thickness);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    Arrows arrows;
};

}  // namespace _geometry

namespace geometry {

using _geometry::Triad;

std::shared_ptr<Triad> triad(float scale = 1.0, float thickness = 0.1);

}  // namespace geometry

}  // namespace slamd