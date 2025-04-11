#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glm/glm.hpp>
#include <optional>
#include <slamd/constants.hpp>
#include <slamd/data/mesh.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/shaders.hpp>
#include <slamd/thread_box.hpp>
#include <thread>
#include <vector>

namespace slamd {
namespace _geometry {

// TODO: move to data

class SimpleMesh : public Geometry {
   public:
    SimpleMesh(
        const data::ColoredMesh& mesh_data,
        float min_brightness = _const::default_min_brightness
    );

    SimpleMesh(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec3>& vertex_colors,
        const std::vector<uint32_t>& triangle_indices,
        float min_brightness = _const::default_min_brightness
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

    data::ColoredMesh mesh_data;

    float min_brightness;
};

}  // namespace _geometry

namespace geometry {

using _geometry::SimpleMesh;
std::shared_ptr<SimpleMesh> simple_mesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& vertex_colors,
    const std::vector<uint32_t>& triangle_indices
);

std::shared_ptr<SimpleMesh> simple_mesh(const data::ColoredMesh& mesh_data);

}  // namespace geometry
}  // namespace slamd