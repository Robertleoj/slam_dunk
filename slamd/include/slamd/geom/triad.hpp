#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/geom/arrows.hpp>
#include <slamd/geom/geometry.hpp>

namespace slamd {
namespace _geom {

class Triad : public Geometry {
   public:
    Triad(float scale, float thickness);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    Arrows arrows;
};

}  // namespace _geom

namespace geom {

using _geom::Triad;

std::shared_ptr<Triad> triad(float scale = 1.0, float thickness = 0.1);

}  // namespace geom

}  // namespace slamd