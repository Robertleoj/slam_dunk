#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glm/glm.hpp>
#include <optional>
#include <slamd/geometry/geometry.hpp>
#include <slamd/shaders.hpp>
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
    static std::optional<ShaderProgram> shader;
    gl::GLuint vao_id;
    gl::GLuint vbo_id;
    gl::GLuint eab_id;

    size_t num_vertices;
};
}  // namespace geometry
}  // namespace slamd