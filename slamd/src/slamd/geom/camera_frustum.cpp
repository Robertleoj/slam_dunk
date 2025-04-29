#include <glm/glm.hpp>
#include <slamd/geom/camera_frustum.hpp>
#include <slamd/global_object_map.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {
namespace _geom {

CameraFrustum::CameraFrustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    data::Image&& image,
    float scale
)
    : img(std::move(image)),
      intrinsics_matrix(intrinsics_matrix),
      image_width(image_width),
      image_height(image_height),
      scale(scale) {}

CameraFrustum::CameraFrustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale
)
    : intrinsics_matrix(intrinsics_matrix),
      image_width(image_width),
      image_height(image_height),
      scale(scale) {}

flatbuffers::Offset<slamd::flatb::Geometry> CameraFrustum::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto frustum_builder = flatb::CameraFrustumBuilder(builder);

    auto intrinsics_fb = gmath::serialize(this->intrinsics_matrix);
    frustum_builder.add_intrinsics(&intrinsics_fb);

    frustum_builder.add_image_height(this->image_height);
    frustum_builder.add_image_width(this->image_width);

    frustum_builder.add_scale(this->scale);

    if (this->img.has_value()) {
        auto img_fb = this->img.value().serialize(builder);
        frustum_builder.add_image(img_fb);
    }

    auto frustum_fb = frustum_builder.Finish();

    return flatb::CreateGeometry(
        builder,
        flatb::GeometryUnion_camera_frustum,
        frustum_fb.Union()
    );
}

}  // namespace _geom

namespace geom {

CameraFrustumPtr camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    data::Image&& image,
    float scale
) {
    auto cam = std::make_shared<_geom::CameraFrustum>(
        intrinsics_matrix,
        image_width,
        image_height,
        std::move(image),
        scale
    );
    // _global::geometries.add(cam->id, cam);

    return cam;
}

CameraFrustumPtr camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale
) {
    auto cam = std::make_shared<_geom::CameraFrustum>(
        intrinsics_matrix,
        image_width,
        image_height,
        scale
    );
    // _global::geometries.add(cam->id, cam);

    return cam;
}

}  // namespace geom
}  // namespace slamd
