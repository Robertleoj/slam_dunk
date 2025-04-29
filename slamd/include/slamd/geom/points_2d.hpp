#pragma once

#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mesh.hpp>
#include <slamd/global_object_map.hpp>

namespace slamd {
namespace _geom {

class Points2D : public Geometry {
   public:
    Points2D(
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<float>& radii
    );

    flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    ) override;

   private:
    std::vector<glm::vec2> positions;
    std::vector<glm::vec3> colors;
    std::vector<float> radii;
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

    auto points_2d = std::make_shared<_geom::Points2D>(
        positions,
        std::move(final_colors),
        std::move(final_radii)
    );
    // _global::geometries.add(points_2d->id, points_2d);
    return points_2d;
}

}  // namespace geom2d
}  // namespace slamd