#pragma once
#include <slamd/geometry/geometry.hpp>
#include <slamd/geometry/poly_line.hpp>

namespace slamd {
namespace _geometry {

class CameraFrustum : public Geometry {
   public:
    CameraFrustum(
        glm::mat3 intrinsics_matrix,
        size_t image_width,
        size_t image_height,
        float scale = 1.0
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    std::vector<PolyLine> poly_lines;
};
}  // namespace _geometry

namespace geometry {

using _geometry::CameraFrustum;

std::shared_ptr<CameraFrustum> camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale = 1.0
);

}  // namespace geometry
}  // namespace slamd