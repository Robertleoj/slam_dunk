#include <slamd_common/gmath/transforms.hpp>
#include <slamd_window/gen/shader_sources.hpp>
#include <slamd_window/geom/image.hpp>
#include <slamd_window/paths.hpp>

namespace slamd {
namespace _geom {

std::shared_ptr<Image> Image::deserialize(
    const slamd::flatb::Image* image_fb
) {
    return std::make_shared<Image>(
        slamd::data::Image::deserialize(image_fb->img()),
        image_fb->normalized()
    );
}

void Image::initialize() {
    // clang-format off
    std::vector<float> verts = {
        // top left
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,

        // top right
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,

        // bottom right
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f,

        // bottom left
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f
    };

    std::vector<uint32_t> indices = {
        // top right triangle
        0, 1, 2,
        // bottom left triangle
        0, 2, 3
    };
    // clang-format on

    gl::glGenVertexArrays(1, &this->vao_id);
    gl::glBindVertexArray(this->vao_id);

    gl::glGenBuffers(1, &this->vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->vbo_id);

    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        verts.size() * sizeof(float),
        verts.data(),
        gl::GL_STATIC_DRAW
    );

    // make the element array buffer
    gl::glGenBuffers(1, &this->eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, this->eab_id);

    gl::glBufferData(
        gl::GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(uint32_t),
        indices.data(),
        gl::GL_STATIC_DRAW
    );

    // bind the vertices
    gl::glVertexAttribPointer(
        0,
        3,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        3 * sizeof(float) + 2 * sizeof(float),
        (void*)0
    );
    gl::glEnableVertexAttribArray(0);

    // and bind the texture coordinates
    gl::glVertexAttribPointer(
        1,
        2,
        gl::GL_FLOAT,
        gl::GL_FALSE,
        3 * sizeof(float) + 2 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    gl::glEnableVertexAttribArray(1);

    gl::glBindVertexArray(0);
}

Image::Image(
    const slamd::data::Image& image,
    bool normalized
)
    : shader(shader_source::image::vert, shader_source::image::frag) {
    this->texture = std::make_unique<graphics::ImageTexture>(image);

    float x_size = static_cast<float>(image.width);
    float y_size = static_cast<float>(image.height);

    if (normalized) {
        const float bigger = std::fmax(x_size, y_size);
        x_size /= bigger;
        y_size /= bigger;
    }

    this->scale = glm::vec2(x_size, y_size);
    this->initialize();
}

void Image::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    gl::glBindVertexArray(this->vao_id);

    this->shader.use();
    this->shader.set_uniform(
        "model",
        model * slamd::gmath::scale_xy(this->scale)
    );
    this->shader.set_uniform("view", view);
    this->shader.set_uniform("projection", projection);

    this->texture->bind();

    gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);

    gl::glBindVertexArray(0);
};

std::optional<slamd::gmath::AABB> Image::bounds() {
    float x_min = 0.0f;
    float x_max = this->scale.x;

    float y_min = 0.0;
    float y_max = this->scale.y;

    return slamd::gmath::AABB(
        glm::vec3(x_min, y_min, 0.0),
        glm::vec3(x_max, y_max, 0.0f)
    );
}

Image::~Image() {
    if (eab_id) {
        gl::glDeleteBuffers(1, &eab_id);
        eab_id = 0;
    }
    if (vbo_id) {
        gl::glDeleteBuffers(1, &vbo_id);
        vbo_id = 0;
    }
    if (vao_id) {
        gl::glDeleteVertexArrays(1, &vao_id);
        vao_id = 0;
    }
}

}  // namespace _geom

namespace geom {

ImagePtr image(
    slamd::data::Image&& image
) {
    return std::make_shared<_geom::Image>(std::move(image), true);
}
}  // namespace geom

namespace geom2d {

ImagePtr image(
    slamd::data::Image&& image
) {
    return std::make_shared<_geom::Image>(std::move(image), false);
}

}  // namespace geom2d

}  // namespace slamd