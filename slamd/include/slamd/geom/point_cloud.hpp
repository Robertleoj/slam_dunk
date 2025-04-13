#pragma once

#include <slamd/geom/geometry.hpp>
#include <slamd/geom/simple_mesh.hpp>
#include <slamd/thread_box.hpp>

namespace slamd {
namespace _geom {

class PointCloud : public Geometry {
   public:
    PointCloud(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

   private:
    // SimpleMesh mesh;

    void initialize();
    std::tuple<size_t, uint, uint> initialize_sphere_mesh();
    uint initialize_pos_buffer();
    uint initialize_radii_buffer();
    uint initialize_color_buffer();

   private:
    struct GLData {
        ShaderProgram shader;
        uint vao_id;
        uint mesh_vbo_id;
        uint mesh_eab_id;
        uint pos_vbo_id;
        uint radii_vbo_id;
        uint colors_vbo_id;
        size_t ball_vertex_count;
    };

    std::optional<ThreadBox<GLData>> gl_data;
    std::optional<std::thread::id> render_thread_id;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;
    std::vector<float> radii;

    // static SimpleMesh make_mesh(
    //     const std::vector<glm::vec3>& positions,
    //     const std::vector<glm::vec3>& colors,
    //     const std::vector<float>& radii
    // );
};

}  // namespace _geom

namespace geom {
using _geom::PointCloud;

template <typename ColorType, typename RadiiType>
std::shared_ptr<PointCloud> point_cloud(
    const std::vector<glm::vec3>& positions,
    const ColorType& colors,
    const RadiiType& radii
) {
    std::vector<glm::vec3> final_colors;
    std::vector<float> final_radii;

    // Handle colors
    if constexpr (std::is_same_v<ColorType, glm::vec3>) {
        final_colors = std::vector<glm::vec3>(positions.size(), colors);
    } else {
        final_colors = colors;
    }

    // Handle radii
    if constexpr (std::is_same_v<RadiiType, float>) {
        final_radii = std::vector<float>(positions.size(), radii);
    } else {
        final_radii = radii;
    }

    return std::make_shared<PointCloud>(
        positions,
        std::move(final_colors),
        std::move(final_radii)
    );
}

}  // namespace geom
}  // namespace slamd