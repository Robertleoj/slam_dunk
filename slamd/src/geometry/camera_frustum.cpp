#include <glm/glm.hpp>
#include <slamd/geometry/camera_frustum.hpp>

namespace slamd {
namespace _geometry {

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
        return glm::vec3(nx, ny, 1.0f) * scale;
    };

    glm::vec3 cam_origin(0.0f);

    glm::vec3 tl = pixel_to_camera(0, 0);
    glm::vec3 tr = pixel_to_camera(w, 0);
    glm::vec3 br = pixel_to_camera(w, h);
    glm::vec3 bl = pixel_to_camera(0, h);

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

    // Notch triangle pointing "up" in camera space (-Y direction)
    glm::vec3 top_center = (tl + tr) * 0.5f;

    float notch_height = scale * 0.15f;
    float notch_width = scale * 0.3f;
    float float_dist = scale * 0.02;
    glm::vec3 float_transl(0.0f, -float_dist, 0.0f);

    glm::vec3 notch_tip =
        top_center + float_transl +
        glm::vec3(0.0f, -notch_height, 0.0f);  // -Y = up in camera space
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
}

void CameraFrustum::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    for (auto& polyline : poly_lines) {
        polyline.render(model, view, projection);
    }
}
}  // namespace _geometry

namespace geometry {
std::shared_ptr<CameraFrustum> camera_frustum(
    glm::mat3 intrinsics_matrix,
    size_t image_width,
    size_t image_height,
    float scale
) {
    return std::make_shared<CameraFrustum>(
        intrinsics_matrix,
        image_width,
        image_height,
        scale
    );
}

}  // namespace geometry
}  // namespace slamd
