#pragma once

#include <memory>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/shaders.hpp>

namespace slamd {
namespace _geom {

/**
 * TODO: refactor to use MonoInstanced
 */
class PointCloud : public Geometry {
   public:
    PointCloud(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii
    );
    ~PointCloud();

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    static std::shared_ptr<PointCloud> deserialize(
        const slamd::flatb::PointCloud* point_cloud_fb
    );

    void update_positions(const std::vector<glm::vec3>& positions);
    void update_colors(const std::vector<glm::vec3>& colors);
    void update_radii(const std::vector<float>& radii);

   private:
    // SimpleMesh mesh;

    void initialize();
    std::tuple<size_t, uint, uint> initialize_sphere_mesh();
    uint initialize_pos_buffer();
    uint initialize_radii_buffer();
    uint initialize_color_buffer();
    void handle_updates();

   private:
    ShaderProgram shader;
    uint vao_id = 0;
    uint mesh_vbo_id = 0;
    uint mesh_eab_id = 0;
    uint pos_vbo_id = 0;
    uint radii_vbo_id = 0;
    uint colors_vbo_id = 0;
    size_t ball_vertex_count;

    std::vector<glm::vec3> positions;
    bool pending_pos_update;

    std::vector<glm::vec3> colors;
    bool pending_colors_update;

    std::vector<float> radii;
    bool pending_radii_update;
};

}  // namespace _geom

namespace geom {
using PointCloudPtr = std::shared_ptr<_geom::PointCloud>;

template <typename ColorType, typename RadiiType>
PointCloudPtr point_cloud(
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

    return std::make_shared<_geom::PointCloud>(
        positions,
        std::move(final_colors),
        std::move(final_radii)
    );
}

}  // namespace geom
}  // namespace slamd