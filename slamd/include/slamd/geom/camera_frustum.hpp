#pragma once
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/image.hpp>
#include <slamd/geom/poly_line.hpp>

namespace slamd {
namespace _geom {

class CameraFrustum : public Geometry {
   public:
    CameraFrustum(
        glm::mat3 intrinsics_matrix,
        size_t image_width,
        size_t image_height,
        float scale = 1.0
    );

    CameraFrustum(
        glm::mat3 intrinsics_matrix,
        size_t image_width,
        size_t image_height,
        data::Image&& image,
        float scale = 1.0
    );
};
}  // namespace _geom

namespace geom {

using CameraFrustumPtr = std::shared_ptr<_geom::CameraFrustum>;

CameraFrustumPtr camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    data::Image&& image,
    float scale = 1.0
);

CameraFrustumPtr camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale = 1.0
);

}  // namespace geom
}  // namespace slamd