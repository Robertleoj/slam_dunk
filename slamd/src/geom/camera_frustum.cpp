#include <glm/glm.hpp>
#include <slamd/geom/camera_frustum.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {
namespace _geom {

CameraFrustum::CameraFrustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    data::Image&& image,
    float scale
) {}

CameraFrustum::CameraFrustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale
) {}

}  // namespace _geom

namespace geom {

CameraFrustumPtr camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    data::Image&& image,
    float scale
) {
    return std::make_shared<_geom::CameraFrustum>(
        intrinsics_matrix,
        image_width,
        image_height,
        std::move(image),
        scale
    );
}

CameraFrustumPtr camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale
) {
    return std::make_shared<_geom::CameraFrustum>(
        intrinsics_matrix,
        image_width,
        image_height,
        scale
    );
}

}  // namespace geom
}  // namespace slamd
