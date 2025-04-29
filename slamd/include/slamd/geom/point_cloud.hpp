#pragma once

#include <memory>
#include <slamd/geom/geometry.hpp>
#include <slamd/global_object_map.hpp>

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

    flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    ) override;

    void update_positions(const std::vector<glm::vec3>& positions);
    void update_colors(const std::vector<glm::vec3>& colors);
    void update_radii(const std::vector<float>& radii);

   private:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;
    std::vector<float> radii;
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

    auto pc = std::make_shared<_geom::PointCloud>(
        positions,
        std::move(final_colors),
        std::move(final_radii)
    );

    // _global::geometries.add(pc->id, pc);
    return pc;
}

}  // namespace geom
}  // namespace slamd