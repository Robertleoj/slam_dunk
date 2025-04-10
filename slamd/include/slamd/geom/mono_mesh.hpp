#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glm/glm.hpp>
#include <optional>
#include <slamd/constants.hpp>
#include <slamd/data/mesh.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/shaders.hpp>
#include <slamd/thread_box.hpp>
#include <vector>

namespace slamd {
namespace _geom {

class MonoMesh : public Geometry {
   public:
    MonoMesh(
        const std::vector<glm::vec3>& vertices,
        const std::vector<uint32_t>& triangle_indices,
        const glm::vec3& color,
        float min_brightness = _const::default_min_brightness
    );

    MonoMesh(
        const data::Mesh& mesh_data,
        const glm::vec3& color,
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

    glm::vec3 color;
    data::Mesh mesh_data;

    float min_brightness;
};

}  // namespace _geom

namespace geom {

using _geom::MonoMesh;

std::shared_ptr<MonoMesh> mono_mesh(
    std::vector<glm::vec3> vertices,
    std::vector<uint32_t> triangle_indices,
    glm::vec3 color
);

}  // namespace geom
}  // namespace slamd