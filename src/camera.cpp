#include <glm/gtc/matrix_transform.hpp>
#include <slam_dunk/camera.hpp>

namespace slamd {

Camera::Camera(
    double fov,
    double near_plane,
    double far_plane
)
    : fov(fov),
      near_plane(near_plane),
      far_plane(far_plane) {}

glm::mat4 Camera::get_projection_matrix(
    double aspect_ratio
) const {
    return glm::perspective(
        glm::radians(this->fov), aspect_ratio, near_plane, far_plane
    );
}

}  // namespace slamd