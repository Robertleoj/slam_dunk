#pragma once

#include <memory>
#include <slamd_window/data/mesh.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/shaders.hpp>
#include <thread>

namespace slamdw {
namespace _geom {

class MonoInstanced : public Geometry {
   public:
    MonoInstanced(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec3>& normals,
        const std::vector<uint32_t>& triangle_indices,
        const std::vector<glm::mat4>& transforms,
        const std::vector<glm::vec3>& colors
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    void update_transforms(const std::vector<glm::mat4>& positions);
    void update_colors(const std::vector<glm::vec3>& colors);

   private:
    void initialize();
    std::tuple<uint, uint> initialize_mesh();
    uint initialize_trans_buffer();
    uint initialize_color_buffer();
    void handle_updates();

   private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> triangle_indices;

    ShaderProgram shader;
    uint vao_id = 0;
    uint mesh_vbo_id = 0;
    uint mesh_eab_id = 0;
    uint trans_vbo_id = 0;
    uint colors_vbo_id = 0;

    std::vector<glm::mat4> transforms;
    bool pending_trans_update;

    std::vector<glm::vec3> colors;
    bool pending_colors_update;
};

}  // namespace _geom
}  // namespace slamd