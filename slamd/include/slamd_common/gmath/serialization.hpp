#pragma once

#include <flatb/gmath_generated.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slamd {
namespace gmath {

slamd::flatb::Mat4 serialize_mat4(
    glm::mat4 mat
) {
    slamd::flatb::Mat4 mat4_fb;

    std::memcpy(
        (void*)mat4_fb.data()->data(),
        glm::value_ptr(mat),
        sizeof(float) * 16
    );

    return mat4_fb;
}

}  // namespace gmath
}  // namespace slamd