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
}  // namespace geom

}  // namespace slamd