#include <numbers>
#include <ranges>
#include <slamd/geom/points_2d.hpp>
#include <slamd_common/gmath/serialization.hpp>

namespace slamd {
namespace _geom {

Points2D::Points2D(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
)
    : positions(positions),
      colors(colors),
      radii(radii) {}

flatbuffers::Offset<slamd::flatb::Geometry> Points2D::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto points_2d_fb = flatb::CreatePoints2D(
        builder,
        gmath::serialize_vector(builder, this->positions),
        gmath::serialize_vector(builder, this->colors),
        gmath::serialize_vector(builder, this->radii)
    );

    return flatb::CreateGeometry(
        builder,
        flatb::GeometryUnion_points_2d,
        points_2d_fb.Union()
    );
}

}  // namespace _geom
}  // namespace slamd