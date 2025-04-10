#pragma once

#include <glm/glm.hpp>
#include <slamd/geom/aabb.hpp>

namespace slamd {

class Camera2D {
   public:
    Camera2D(const _geom::AABB2D& viewport);

    void set_viewport(const _geom::AABB2D& viewport);

    glm::mat4 get_projection_matrix() const;

    _geom::AABB2D viewport;

    void translate_relative(glm::vec2 relative_translation);
};

}  // namespace slamd