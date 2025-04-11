#pragma once

#include <slamd/geom/geometry.hpp>
#include <slamd/geom/simple_mesh.hpp>

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
    SimpleMesh mesh;

    static SimpleMesh make_mesh(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii
    );
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