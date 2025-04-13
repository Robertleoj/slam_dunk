#pragma once

#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mesh.hpp>

namespace slamd {
namespace _geom {

class Points2D : public Geometry {
   public:
    Points2D(
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii
    );

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

    std::optional<gmath::AABB> bounds() override;

   private:
    Mesh mesh;
    gmath::AABB cached_bounds;
};

}  // namespace _geom

namespace geom2d {
using _geom::Points2D;

template <typename ColorType, typename RadiiType>
std::shared_ptr<Points2D> points_2d(
    const std::vector<glm::vec2>& positions,
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

    return std::make_shared<Points2D>(
        positions,
        std::move(final_colors),
        std::move(final_radii)
    );
}

}  // namespace geom2d
}  // namespace slamd