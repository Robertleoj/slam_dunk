#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/camera_2d.hpp>

namespace slamd {

Camera2D::Camera2D(
    const _geom::Rect2D& viewport
)
    : viewport(viewport) {}

glm::mat4 Camera2D::get_projection_matrix() const {
    // Left, Right, Bottom, Top, Near, Far
    return glm::ortho(
        this->viewport.top_left.x,
        this->viewport.bottom_right.x,
        this->viewport.bottom_right.y,
        this->viewport.top_left.y,
        -1.0f,
        1.0f
    );
}

void Camera2D::set_viewport(
    const _geom::Rect2D& viewport
) {
    this->viewport = _geom::Rect2D(viewport);
}

void Camera2D::translate_relative(
    glm::vec2 relative_translation
) {
    glm::vec2 viewport_size = this->viewport.size();

    float smaller = glm::min(viewport_size.x, viewport_size.y);

    glm::vec2 translation = relative_translation * smaller;

    this->viewport = this->viewport.translate(translation);
}

void Camera2D::zoom_relative(
    float amount,
    std::optional<glm::vec2> maybe_normalized_center
) {
    glm::vec2 viewport_size = this->viewport.size();

    float smaller = glm::min(viewport_size.x, viewport_size.y);

    glm::vec2 normalized_center =
        maybe_normalized_center.value_or(glm::vec2(0.5f, -0.5f));

    spdlog::debug(
        "Normalized center {} {}",
        normalized_center.x,
        normalized_center.y
    );

    glm::vec2 center = this->viewport.unnormalize(normalized_center);

    spdlog::debug("unnormalized center {} {}", center.x, center.y);

    float zoom_factor = 1.0f - amount;
    glm::vec2 new_size = viewport_size * zoom_factor;

    this->viewport = _geom::Rect2D::from_center_size(center, new_size);
}


void Camera2D::translate_normalized(
    glm::vec2 normalized_translation
) {
    glm::vec2 unnormalized = this->viewport.size() * normalized_translation;
    spdlog::debug("unnormalized {} {}", unnormalized.x, unnormalized.y);
    this->viewport = this->viewport.translate(unnormalized);
}

}  // namespace slamd