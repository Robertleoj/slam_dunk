#pragma once
#include <flatb/geometry_generated.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <slamd_window/geom/arrows.hpp>
#include <slamd_window/geom/geometry.hpp>

namespace slamd {
namespace _geom {

class Triad : public Geometry {
   public:
    Triad(float scale, float thickness, glm::mat4 pose);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    static std::shared_ptr<Triad> deserialize(
        const slamd::flatb::Triad* triad_fb
    );

   private:
    std::unique_ptr<Arrows> arrows;
    glm::mat4 pose;
    glm::mat4 scale_transform;
};

}  // namespace _geom
}  // namespace slamd