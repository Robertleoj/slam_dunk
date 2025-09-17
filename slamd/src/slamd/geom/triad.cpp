#include <slamd/geom/triad.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {
namespace _geom {

Triad::Triad(
    float scale,
    float thickness,
    std::optional<glm::mat4> pose
)
    : scale(scale),
      thickness(thickness),
      pose(pose.value_or(glm::mat4(1.0))) {}

flatbuffers::Offset<slamd::flatb::Geometry> Triad::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto pose_fb = gmath::serialize(pose);

    auto triad_fb =
        flatb::CreateTriad(builder, this->scale, this->thickness, &pose_fb);

    return flatb::CreateGeometry(
        builder,
        this->id.value,
        flatb::GeometryUnion_triad,
        triad_fb.Union()
    );
}

}  // namespace _geom

namespace geom {

TriadPtr triad(
    float scale,
    float thickness
) {
    auto triad = std::make_shared<_geom::Triad>(scale, thickness);
    // _global::geometries.add(triad->id, triad);
    return triad;
}

TriadPtr triad(
    glm::mat4 pose,
    float scale,
    float thickness
) {
    auto triad = std::make_shared<_geom::Triad>(scale, thickness, pose);
    // _global::geometries.add(triad->id, triad);
    return triad;
}
}  // namespace geom

}  // namespace slamd