#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/transforms.hpp>
#include <slamd_window/geom/camera_frustum.hpp>

namespace slamd {
namespace _geom {

std::shared_ptr<CameraFrustum> CameraFrustum::deserialize(
    const slamd::flatb::CameraFrustum* frustum_fb
) {
    auto intrinsics = slamd::gmath::deserialize(frustum_fb->intrinsics());

    if (frustum_fb->image()) {
        return std::make_shared<CameraFrustum>(
            intrinsics,
            frustum_fb->image_width(),
            frustum_fb->image_height(),
            slamd::data::Image::deserialize(frustum_fb->image()),
            frustum_fb->scale()
        );
    } else {
        return std::make_shared<CameraFrustum>(
            intrinsics,
            frustum_fb->image_width(),
            frustum_fb->image_height(),
            frustum_fb->scale()
        );
    }
}

CameraFrustum::CameraFrustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    slamd::data::Image&& image,
    float scale
)
    : CameraFrustum(intrinsics_matrix, image_width, image_height, scale) {
    // we assume the image has the same aspect as the frustum
    // so we just need to scale it by a single number
    // we can just use the larger

    spdlog::info("Got image of shape (wh) {}, {}", image.width, image.height);
    spdlog::info(
        "Expecting {} pixels, got {}",
        image.width * image.height * image.channels,
        image.data.size()
    );

    // spdlog::info("found zero bytes: {}", image.data[100000] == 0);

    float image_scale = std::fmax(
        this->corners.tr.x - this->corners.tl.x,
        this->corners.bl.y - this->corners.tr.y
    );
    glm::mat4 scale_transform =
        slamd::gmath::scale_xy(glm::vec2(image_scale, image_scale));
    glm::mat4 translate = slamd::gmath::t3D(this->corners.tl);

    this->cam_image.emplace(
        Image(std::move(image), true),
        translate * scale_transform
    );
}

CameraFrustum::CameraFrustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale
) {
    float fx = intrinsics_matrix[0][0];
    float fy = intrinsics_matrix[1][1];
    float cx = intrinsics_matrix[2][0];
    float cy = intrinsics_matrix[2][1];

    float w = static_cast<float>(image_width);
    float h = static_cast<float>(image_height);

    auto pixel_to_camera = [&](float x, float y) -> glm::vec3 {
        float nx = (x - cx) / fx;
        float ny = (y - cy) / fy;
        return glm::vec3(nx, ny, 1.0f);
    };

    glm::vec3 cam_origin(0.0f);

    glm::vec3 tl = pixel_to_camera(0, 0);
    glm::vec3 tr = pixel_to_camera(w, 0);
    glm::vec3 br = pixel_to_camera(w, h);
    glm::vec3 bl = pixel_to_camera(0, h);

    this->corners = {tl, tr, br, bl};

    float thickness = 0.01f;
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    float min_brightness = 0.9f;

    std::vector<std::pair<glm::vec3, glm::vec3>> segments = {
        {cam_origin, tl},
        {cam_origin, tr},
        {cam_origin, br},
        {cam_origin, bl},
        {tl, tr},
        {tr, br},
        {br, bl},
        {bl, tl}
    };

    glm::vec3 top_center = (tl + tr) * 0.5f;

    float notch_height = 0.15f;
    float notch_width = 0.3f;
    float float_dist = 0.02;
    glm::vec3 float_transl(0.0f, -float_dist, 0.0f);

    glm::vec3 notch_tip =
        top_center + float_transl + glm::vec3(0.0f, -notch_height, 0.0f);
    glm::vec3 notch_left =
        top_center + float_transl + glm::vec3(-notch_width * 0.5f, 0.0f, 0.0f);
    glm::vec3 notch_right =
        top_center + float_transl + glm::vec3(notch_width * 0.5f, 0.0f, 0.0f);

    segments.push_back({notch_left, notch_tip});
    segments.push_back({notch_tip, notch_right});
    segments.push_back({notch_right, notch_left});

    for (const auto& [start, end] : segments) {
        this->poly_lines.emplace_back(
            std::vector<glm::vec3>{start, end},
            thickness,
            color,
            min_brightness
        );
    }

    this->scale_transform = slamd::gmath::scale_all(scale);
}

void CameraFrustum::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    model = model * this->scale_transform;
    for (auto& polyline : poly_lines) {
        polyline.render(model, view, projection);
    }

    if (this->cam_image.has_value()) {
        auto& img = this->cam_image.value().image_geometry;
        auto& transform = this->cam_image.value().transform;
        img.render(model * transform, view, projection);
    }
}
}  // namespace _geom

namespace geom {

CameraFrustumPtr camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    slamd::data::Image&& image,
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
