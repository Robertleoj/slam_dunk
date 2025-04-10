#include <glm/gtc/matrix_transform.hpp>
#include <slamd/camera_2d.hpp>

namespace slamd {
Camera2D::Camera2D(
    Bounds x_bounds,
    Bounds y_bounds
)
    : x_bounds(x_bounds),
      y_bounds(y_bounds) {}

glm::mat4 Camera2D::get_projection_matrix() const {
    // Left, Right, Bottom, Top, Near, Far
    return glm::ortho(
        x_bounds.min,
        x_bounds.max,
        y_bounds.min,
        y_bounds.max,
        -1.0f,
        1.0f
    );
}

void Camera2D::set_bounds(
    Bounds x_bounds,
    Bounds y_bounds
) {
    this->x_bounds = x_bounds;
    this->y_bounds = y_bounds;
}

}  // namespace slamd