#pragma once

#include <glm/glm.hpp>
#include <slamd_common/gmath/angle.hpp>

namespace slamd {

class Arcball {
   public:
    glm::mat4 center;
    gmath::Angle theta;
    gmath::Angle phi;
    float radius;

   public:
    Arcball();
    void rotate(gmath::Angle x, gmath::Angle y);
    void translate_relative(glm::vec3 relative_amount);
    void zoom(float factor);
    void reset();
    glm::mat4 camera_in_center() const;
    glm::mat4 view_matrix() const;

    inline static const gmath::Angle default_theta = gmath::Angle::deg(45.0);
    inline static const gmath::Angle default_phi = gmath::Angle::deg(-20.0);

    inline static const glm::mat4 default_center = glm::mat4(1.0);
    inline static const float default_radius = 10.0f;
};

}  // namespace slamd