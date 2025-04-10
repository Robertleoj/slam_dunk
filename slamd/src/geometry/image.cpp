#include <filesystem>
#include <slamd/assert.hpp>
#include <slamd/geometry/image.hpp>
#include <slamd/paths.hpp>

namespace fs = std::filesystem;

namespace slamd {
namespace _geometry {

const fs::path vertex_shader_path =
    shader_folder() / "image" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "image" / "fragment_shader.frag";

void Image::initialize() {
    assert_thread(this->render_thread_id.value());

    // clang-format off
    std::vector<float> verts = {
        // top left
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,

        // top right
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,

        // bottom right
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f,

        // bottom left
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f
    };

    std::vector<uint32_t> indices = {
        // top right triangle
        0, 1, 2,
        // bottom left triangle
        0, 2, 3
    };
    // clang-format on

    gl::GLuint vao_id;
    gl::GLuint vbo_id;
    gl::GLuint eab_id;

    gl::glGenVertexArrays(1, &vao_id);
    gl::glBindVertexArray(vao_id);

    gl::glGenBuffers(1, &vbo_id);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vbo_id);

    gl::glBufferData(
        gl::GL_ARRAY_BUFFER,
        verts.size() * sizeof(float),
        verts.data(),
        gl::GL_STATIC_DRAW
    );

    // make the element array buffer
    gl::glGenBuffers(1, &eab_id);
    gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, eab_id);

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

    GLData gl_data = {
        vao_id,
        vbo_id,
        eab_id,
        graphics::ImageTexture(this->image),
        ShaderProgram(vertex_shader_path, fragment_shader_path)
    };

    this->gl_data.emplace(gl_data);
}

Image::Image(
    data::Image&& image
)
    : image(image) {}

void Image::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    if (!this->render_thread_id.has_value()) {
        this->render_thread_id = std::this_thread::get_id();
        this->initialize();
    }

    auto gl_data = this->gl_data.value().get();

    gl::glBindVertexArray(gl_data->vao_id);

    gl_data->shader.use();
    gl_data->shader.setUniform("model", model);
    gl_data->shader.setUniform("view", view);
    gl_data->shader.setUniform("projection", projection);

    gl_data->texture.bind();

    gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);

    gl::glBindVertexArray(0);
};

}  // namespace _geometry

namespace geometry {

std::shared_ptr<Image> image(
    data::Image&& image
) {
    return std::make_shared<Image>(std::move(image));
}
}  // namespace geometry

namespace geometry2d {

std::shared_ptr<Image> image(
    data::Image&& image
) {
    return std::make_shared<Image>(std::move(image));
}

}  // namespace geometry2d

}  // namespace slamd