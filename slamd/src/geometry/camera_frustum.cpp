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

    // Lines from camera to image corners
    this->poly_lines
        .emplace_back(std::vector<glm::vec3>{cam_origin, tl}, thickness, color);

    this->poly_lines
        .emplace_back(std::vector<glm::vec3>{cam_origin, tr}, thickness, color);

    this->poly_lines
        .emplace_back(std::vector<glm::vec3>{cam_origin, br}, thickness, color);

    this->poly_lines
        .emplace_back(std::vector<glm::vec3>{cam_origin, bl}, thickness, color);

    // Rectangle around image plane
    this->poly_lines
        .emplace_back(std::vector<glm::vec3>{tl, tr}, thickness, color);

    this->poly_lines
        .emplace_back(std::vector<glm::vec3>{tr, br}, thickness, color);

    this->poly_lines
        .emplace_back(std::vector<glm::vec3>{br, bl}, thickness, color);

    this->poly_lines
        .emplace_back(std::vector<glm::vec3>{bl, tl}, thickness, color);
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
