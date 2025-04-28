#include <slamd/geom/triad.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {
namespace _geom {

Triad::Triad(
    float scale,
    float thickness
)
    : scale(scale),
      thickness(thickness) {}

flatbuffers::Offset<slamd::flatb::Geometry> Triad::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto triad_fb = flatb::CreateTriad(builder, this->scale, this->thickness);

    return flatb::CreateGeometry(
        builder,
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
    return std::make_shared<_geom::Triad>(scale, thickness);
}
}  // namespace geom

}  // namespace slamd