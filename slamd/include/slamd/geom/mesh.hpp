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

class Mesh : public Geometry {
   public:
    Mesh(
        const data::MeshData& mesh_data,
        float min_brightness = _const::default_min_brightness
    );

    Mesh(
        data::MeshData&& mesh_data,
        float min_brightness = _const::default_min_brightness
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    void update_positions(
        const std::vector<glm::vec3>& positions,
        bool recompute_normals = true
    );
    void update_colors(const std::vector<glm::vec3>& colors);
    void update_normals(const std::vector<glm::vec3>& normals);

   private:
    void handle_updates();
    void maybe_initialize();

   private:
    static thread_local std::optional<ShaderProgram> shader;
    struct GLData {
        uint vao_id;
        uint pos_vbo_id;
        uint color_vbo_id;
        uint normal_vbo_id;
        uint eab_id;
    };

    std::optional<GLData> gl_data;
    std::optional<std::thread::id> render_thread_id;

    data::MeshData mesh_data;
    bool pos_update_pending = false;
    bool color_update_pending = false;
    bool normal_update_pending = false;

    float min_brightness;
};

}  // namespace _geom

namespace geom {

using _geom::Mesh;

std::shared_ptr<Mesh> mesh(const data::MeshData& mesh_data);

}  // namespace geom
}  // namespace slamd