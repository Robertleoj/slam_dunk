#include <format>
#include <slamd/geom/point_cloud.hpp>
#include <slamd_common/gmath/serialization.hpp>

namespace slamd {
namespace _geom {

PointCloud::PointCloud(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
)
    : positions(positions),
      colors(colors),
      radii(radii) {
    if (!((positions.size() == colors.size()) && (colors.size() == radii.size())
        )) {
        throw std::invalid_argument(
            std::format(
                "number of positions, colors, and radii must be the same, got "
                "{} positions, {} colors, {} radii",
                positions.size(),
                colors.size(),
                radii.size()
            )
        );
    }
}

flatbuffers::Offset<slamd::flatb::Geometry> PointCloud::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto point_cloud_fb = flatb::CreatePointCloud(
        builder,
        gmath::serialize_vector(builder, this->positions),
        gmath::serialize_vector(builder, this->colors),
        gmath::serialize_vector(builder, this->radii)
    );

    return flatb::CreateGeometry(
        builder,
        flatb::GeometryUnion_point_cloud,
        point_cloud_fb.Union()
    );
}

void PointCloud::update_positions(
    const std::vector<glm::vec3>& positions
) {}

void PointCloud::update_colors(
    const std::vector<glm::vec3>& colors
) {}

void PointCloud::update_radii(
    const std::vector<float>& radii
) {}

}  // namespace _geom
}  // namespace slamd