#include <glm/gtc/matrix_transform.hpp>
#include <slamd/camera_2d.hpp>

namespace slamd {

Camera2D::Camera2D(
    const _geom::AABB2D& viewport
)
    : viewport(viewport) {}

glm::mat4 Camera2D::get_projection_matrix() const {
    // Left, Right, Bottom, Top, Near, Far
    return glm::ortho(
        this->viewport.min.x,
        this->viewport.max.x,
        this->viewport.min.y,
        this->viewport.max.y,
        -1.0f,
        1.0f
    );
}

void Camera2D::set_viewport(
    const _geom::AABB2D& viewport
) {
    this->viewport = _geom::AABB2D(viewport);
}

void Camera2D::translate_relative(
    glm::vec2 relative_translation
) {
    glm::vec2 viewport_size = this->viewport.size();

    float smaller = glm::min(viewport_size.x, viewport_size.y);

    glm::vec2 translation = relative_translation * smaller;

    this->viewport = this->viewport.translate(translation);
}

}  // namespace slamd