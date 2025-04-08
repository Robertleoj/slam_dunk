#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glm/glm.hpp>
#include <optional>
#include <slamd/geometry/geometry.hpp>
#include <slamd/shaders.hpp>
#include <slamd/thread_box.hpp>
#include <thread>
#include <vector>

namespace slamd {
namespace geometry {

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

class SimpleMesh : public Geometry {
   public:
    SimpleMesh(
        std::vector<Vertex> vertices,
        std::vector<uint32_t> triangle_indices
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    void initialize();

   private:
    static thread_local std::optional<ShaderProgram> shader;
    struct GLData {
        gl::GLuint vao_id;
        gl::GLuint vbo_id;
        gl::GLuint eab_id;
    };

    std::optional<ThreadBox<GLData>> gl_data;
    std::optional<std::thread::id> render_thread_id;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> triangle_indices;
};

std::shared_ptr<SimpleMesh> simple_mesh(
    std::vector<Vertex> vertices,
    std::vector<uint32_t> triangle_indices
);

}  // namespace geometry
}  // namespace slamd