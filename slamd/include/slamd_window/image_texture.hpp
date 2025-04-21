#pragma once

#include <glbinding/gl/gl.h>
#include <slamd/data/image.hpp>

namespace slamd {
namespace graphics {

class ImageTexture {
   public:
    ImageTexture(const data::Image& image);

    const size_t width;
    const size_t height;

    void bind();

   private:
    gl::GLuint texture_id;
};

}  // namespace graphics
}  // namespace slamd