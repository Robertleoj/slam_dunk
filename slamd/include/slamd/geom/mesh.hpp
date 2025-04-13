#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <slamd/constants.hpp>
#include <slamd/data/mesh.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/shaders.hpp>
#include <thread>
#include <vector>

namespace slamd {
namespace _geom {

// TODO: move to data

class Mesh : public Geometry {
   public:
    Mesh(
        const data::ColoredMesh& mesh_data,
        float min_brightness = _const::default_min_brightness
    );

    Mesh(
        const data::Mesh& mesh_data,
        const glm::vec3& color,
        float min_brightness = _const::default_min_brightness
    );

    Mesh(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec3>& vertex_colors,
        const std::vector<uint32_t>& triangle_indices,
        float min_brightness = _const::default_min_brightness
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    void maybe_initialize();

   private:
    static thread_local std::optional<ShaderProgram> shader;
    struct GLData {
        uint vao_id;
        uint vbo_id;
        uint eab_id;
    };

    std::optional<GLData> gl_data;
    std::optional<std::thread::id> render_thread_id;

    data::ColoredMesh mesh_data;

    float min_brightness;
};

}  // namespace _geom

namespace geom {

using _geom::Mesh;

std::shared_ptr<Mesh> mesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& vertex_colors,
    const std::vector<uint32_t>& triangle_indices
);

// std::shared_ptr<Mesh> mesh(
//     const std::vector<glm::vec3>& vertices,
//     const glm::vec3& color,
//     const std::vector<uint32_t>& triangle_indices
// );

std::shared_ptr<Mesh> mesh(const data::ColoredMesh& mesh_data);
// std::shared_ptr<Mesh> mesh(const data::Mesh& mesh_data, const glm::vec3&
// color);

}  // namespace geom
}  // namespace slamd