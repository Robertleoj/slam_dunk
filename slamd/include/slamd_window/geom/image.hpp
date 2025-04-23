#pragma once
#include <memory>
#include <optional>
#include <slamd_common/gmath/aabb.hpp>
#include <slamd_window/data/image.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/image_texture.hpp>
#include <slamd_window/shaders.hpp>
#include <thread>

namespace slamdw {
namespace _geom {

class Image : public Geometry {
   public:
    struct GLData {
        gl::GLuint vao_id;
        gl::GLuint vbo_id;
        gl::GLuint eab_id;
        graphics::ImageTexture texture;
        ShaderProgram shader;
    };

    std::optional<GLData> gl_data;
    std::optional<std::thread::id> render_thread_id;

   private:
    data::Image image;
    glm::vec2 scale;

   public:
    Image(data::Image&& image, bool normalized = true);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    std::optional<slamd::gmath::AABB> bounds() override;

   private:
    void maybe_initialize();
};

}  // namespace _geom

namespace geom {
using ImagePtr = std::shared_ptr<_geom::Image>;

ImagePtr image(data::Image&& image);
}  // namespace geom

namespace geom2d {
using ImagePtr = std::shared_ptr<_geom::Image>;

ImagePtr image(data::Image&& image);

}  // namespace geom2d

}  // namespace slamdw