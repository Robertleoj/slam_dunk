#pragma once

#include <glbinding/gl/gl.h>
#include <slamd_common/data/image.hpp>

namespace slamdw {
namespace graphics {

class ImageTexture {
   public:
    ImageTexture(const slamd::data::Image& image);

    const size_t width;
    const size_t height;

    void bind();

   private:
    gl::GLuint texture_id;
};

}  // namespace graphics
}  // namespace slamd