#pragma once

#include <memory>
#include <slamd/data/mesh.hpp>
#include <slamd/geom/geometry.hpp>
#include <slamd/shaders.hpp>
#include <thread>

namespace slamd {
namespace _geom {

class MonoInstanced : public Geometry {
   public:
    MonoInstanced(
        const data::MeshData& instance_mesh,
        const std::vector<glm::mat4>& transforms,
        const std::vector<glm::vec3>& colors
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    void update_transforms(const std::vector<glm::mat4>& positions);
    void update_colors(const std::vector<glm::vec3>& colors);

   private:
    void maybe_initialize();
    std::tuple<uint, uint> initialize_mesh();
    uint initialize_trans_buffer();
    uint initialize_color_buffer();
    void handle_updates();

   private:
    data::MeshData mesh_data;

    struct GLData {
        ShaderProgram shader;
        uint vao_id;
        uint mesh_vbo_id;
        uint mesh_eab_id;
        uint trans_vbo_id;
        uint colors_vbo_id;
    };

    std::optional<GLData> gl_data;
    std::optional<std::thread::id> render_thread_id;

    std::vector<glm::mat4> transforms;
    bool pending_trans_update;

    std::vector<glm::vec3> colors;
    bool pending_colors_update;
};

}  // namespace _geom
}  // namespace slamd