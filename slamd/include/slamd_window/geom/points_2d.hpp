#pragma once

#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/mesh.hpp>

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

    std::optional<slamd::gmath::AABB> bounds() override;

    static std::shared_ptr<Points2D> deserialize(
        const slamd::flatb::Points2D* points2d_fb
    );

   private:
    std::unique_ptr<Mesh> mesh;
    slamd::gmath::AABB cached_bounds;
};

}  // namespace _geom

namespace geom2d {
using PointsPtr = std::shared_ptr<_geom::Points2D>;

template <typename ColorType, typename RadiiType>
PointsPtr points(
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

    return std::make_shared<_geom::Points2D>(
        positions,
        std::move(final_colors),
        std::move(final_radii)
    );
}

}  // namespace geom2d
}  // namespace slamd