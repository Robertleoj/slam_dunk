#pragma once

#include <glm/glm.hpp>

namespace slamd {

struct Bounds {
    float min;
    float max;
};

class Camera2D {
   public:
    Camera2D(Bounds x_bounds, Bounds y_bounds);

    void set_bounds(Bounds min, Bounds max);

    glm::mat4 get_projection_matrix() const;

    Bounds x_bounds;
    Bounds y_bounds;
};

}  // namespace slamd