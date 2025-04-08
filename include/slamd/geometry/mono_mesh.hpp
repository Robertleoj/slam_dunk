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

class MonoMesh : public Geometry {
   public:
    MonoMesh(
        std::vector<glm::vec3> vertices,
        std::vector<uint32_t> triangle_indices,
        glm::vec3 color
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    static std::optional<ShaderProgram> shader;
    gl::GLuint vao_id;
    gl::GLuint vbo_id;
    gl::GLuint eab_id;

    size_t num_vertices;
    glm::vec3 color;
};

std::shared_ptr<MonoMesh> mono_mesh(
    std::vector<glm::vec3> vertices,
    std::vector<uint32_t> triangle_indices,
    glm::vec3 color
);

}  // namespace geometry
}  // namespace slamd