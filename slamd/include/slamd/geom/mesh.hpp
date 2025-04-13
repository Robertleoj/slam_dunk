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

    void update_mesh(
        std::optional<std::vector<glm::vec3>> positions,
        std::optional<std::vector<glm::vec3>> colors,
        std::optional<std::vector<glm::vec3>> normals,
        bool recompute_normals = false
    );

   private:
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

    bool pos_update_pending;
    bool color_update_pending;
    bool normal_update_pending;

    std::optional<GLData> gl_data;
    std::optional<std::thread::id> render_thread_id;

    data::MeshData mesh_data;

    float min_brightness;
};

}  // namespace _geom

namespace geom {

using _geom::Mesh;

std::shared_ptr<Mesh> mesh(const data::ColoredMesh& mesh_data);

}  // namespace geom
}  // namespace slamd