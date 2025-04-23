#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd_window/camera_2d.hpp>

namespace slamdw {

Camera2D::Camera2D(
    const slamd::gmath::Rect2D& viewport
)
    : viewport(viewport) {}

glm::mat4 Camera2D::get_projection_matrix() const {
    // Left, Right, Bottom, Top, Near, Far
    return glm::ortho(
        this->viewport.top_left.x,
        this->viewport.bottom_right.x,
        this->viewport.bottom_right.y,
        this->viewport.top_left.y,
        -100.0f,
        10000.0f
    );
}

void Camera2D::set_viewport(
    const slamd::gmath::Rect2D& viewport
) {
    this->viewport = slamd::gmath::Rect2D(viewport);
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
    std::optional<glm::vec2> maybe_normalized_mouse_pos
) {
    float zoom_factor = 1.0f - amount;

    glm::vec2 viewport_size = this->viewport.size();

    glm::vec2 new_size = viewport_size * zoom_factor;

    glm::vec2 mouse_world_normalized =
        maybe_normalized_mouse_pos.value_or(glm::vec2(0.5f, 0.5f));

    glm::vec2 mouse_world = this->viewport.unnormalize(mouse_world_normalized);

    glm::vec2 current_top_left = this->viewport.top_left;
    glm::vec2 new_top_left =
        mouse_world * (1.0f - zoom_factor) + current_top_left * zoom_factor;

    this->viewport =
        slamd::gmath::Rect2D(new_top_left, new_top_left + new_size);
}

void Camera2D::translate_normalized(
    glm::vec2 normalized_translation
) {
    glm::vec2 unnormalized = this->viewport.size() * normalized_translation;
    this->viewport = this->viewport.translate(unnormalized);
}

glm::vec2 Camera2D::get_world_coords(
    glm::vec2 normalized_mouse_pos
) {
    return this->viewport.unnormalize(normalized_mouse_pos);
}

}  // namespace slamdw