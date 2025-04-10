#pragma once
#include <optional>
#include <slamd/data/image.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/gmath/aabb.hpp>
#include <slamd/image_texture.hpp>
#include <slamd/shaders.hpp>
#include <slamd/thread_box.hpp>

namespace slamd {
namespace _geometry {

class Image : public Geometry {
   public:
    struct GLData {
        gl::GLuint vao_id;
        gl::GLuint vbo_id;
        gl::GLuint eab_id;
        graphics::ImageTexture texture;
        ShaderProgram shader;
    };

    std::optional<ThreadBox<GLData>> gl_data;
    std::optional<std::thread::id> render_thread_id;

   private:
    data::Image image;
    glm::vec2 scale;

   public:
    Image(data::Image&& image, bool normalized = true);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    std::optional<gmath::AABB> bounds() override;

   private:
    void initialize();
};

}  // namespace _geometry

namespace geometry {
using _geometry::Image;

std::shared_ptr<Image> image(data::Image&& image);
}  // namespace geometry

namespace geometry2d {
using _geometry::Image;

std::shared_ptr<Image> image(data::Image&& image);
}  // namespace geometry2d

}  // namespace slamd