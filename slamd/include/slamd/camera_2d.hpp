#pragma once

#include <glm/glm.hpp>
#include <slamd/geom/rect2d.hpp>

namespace slamd {

class Camera2D {
   public:
    Camera2D(const _geom::Rect2D& viewport);

    void set_viewport(const _geom::Rect2D& viewport);

    glm::mat4 get_projection_matrix() const;

    _geom::Rect2D viewport;

    void translate_relative(glm::vec2 relative_translation);
    void
    zoom_relative(float amount, std::optional<glm::vec2> loc = std::nullopt);
};

}  // namespace slamd