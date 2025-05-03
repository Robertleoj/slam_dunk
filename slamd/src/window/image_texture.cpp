#include <slamd_window/image_texture.hpp>

namespace slamd {
namespace graphics {

ImageTexture::ImageTexture(
    const slamd::data::Image& image
)
    : width(image.width),
      height(image.height),
      texture_id(0) {
    gl::glGenTextures(1, &this->texture_id);
    gl::glBindTexture(gl::GL_TEXTURE_2D, this->texture_id);

    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    gl::glTexParameteri(
        gl::GL_TEXTURE_2D,
        gl::GL_TEXTURE_WRAP_S,
        gl::GL_REPEAT
    );

    gl::glTexParameteri(
        gl::GL_TEXTURE_2D,
        gl::GL_TEXTURE_WRAP_T,
        gl::GL_REPEAT
    );

    gl::glTexParameteri(
        gl::GL_TEXTURE_2D,
        gl::GL_TEXTURE_MIN_FILTER,
        gl::GL_LINEAR_MIPMAP_LINEAR
    );

    gl::glTexParameteri(
        gl::GL_TEXTURE_2D,
        gl::GL_TEXTURE_MAG_FILTER,
        gl::GL_LINEAR
    );

    // load and generate the texture
    gl::glTexImage2D(
        gl::GL_TEXTURE_2D,
        0,
        gl::GL_RGB,
        image.width,
        image.height,
        0,
        gl::GL_RGB,
        gl::GL_UNSIGNED_BYTE,
        image.data.data()
    );
    gl::glGenerateMipmap(gl::GL_TEXTURE_2D);
};

void ImageTexture::bind() {
    gl::glBindTexture(gl::GL_TEXTURE_2D, this->texture_id);
}

}  // namespace graphics

}  // namespace slamd