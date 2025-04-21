#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <slamd_window/constants.hpp>
#include <slamd_window/data/mesh.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/shaders.hpp>
#include <thread>
#include <vector>

namespace slamdw {
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
    void initialize();

   private:
    static thread_local std::optional<ShaderProgram> shader;

    uint vao_id = 0;
    uint pos_vbo_id = 0;
    uint color_vbo_id = 0;
    uint normal_vbo_id = 0;
    uint eab_id = 0;

    data::MeshData mesh_data;
    bool pos_update_pending = false;
    bool color_update_pending = false;
    bool normal_update_pending = false;

    float min_brightness;
};

}  // namespace _geom

namespace geom {

using MeshPtr = std::shared_ptr<_geom::Mesh>;

MeshPtr mesh(const data::MeshData& mesh_data);

}  // namespace geom
}  // namespace slamd