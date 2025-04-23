#pragma once

#include <glm/glm.hpp>
#include <slamd_common/gmath/angle.hpp>

namespace slamdw {

class Arcball {
   public:
    glm::mat4 center;
    slamd::gmath::Angle theta;
    slamd::gmath::Angle phi;
    float radius;

   public:
    Arcball();
    void rotate(slamd::gmath::Angle x, slamd::gmath::Angle y);
    void translate_relative(glm::vec3 relative_amount);
    void zoom(float factor);
    void reset();
    glm::mat4 camera_in_center() const;
    glm::mat4 view_matrix() const;

    inline static const slamd::gmath::Angle default_theta =
        slamd::gmath::Angle::deg(45.0);
    inline static const slamd::gmath::Angle default_phi =
        slamd::gmath::Angle::deg(-20.0);

    inline static const glm::mat4 default_center = glm::mat4(1.0);
    inline static const float default_radius = 10.0f;
};

}  // namespace slamdw