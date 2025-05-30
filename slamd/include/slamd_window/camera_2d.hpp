#pragma once

#include <glm/glm.hpp>
#include <slamd_common/gmath/rect2d.hpp>

namespace slamd {

class Camera2D {
   public:
    Camera2D(const slamd::gmath::Rect2D& viewport);

    void set_viewport(const slamd::gmath::Rect2D& viewport);

    glm::mat4 get_projection_matrix() const;

    slamd::gmath::Rect2D viewport;

    void translate_relative(glm::vec2 relative_translation);
    void zoom_relative(
        float amount,
        std::optional<glm::vec2> normalized_mouse_loc = std::nullopt
    );

    void translate_normalized(glm::vec2 normalized_translation);

    glm::vec2 get_world_coords(glm::vec2 normalized_window_coords);
};

}  // namespace slamd