#include <slamd/geom/box.hpp>
#include <slamd_common/data/mesh.hpp>

namespace slamd {
namespace _geom {

Box::Box() {}

flatbuffers::Offset<slamd::flatb::Geometry> Box::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto box_fb = flatb::CreateBox(builder);
    return flatb::CreateGeometry(
        builder,
        this->id.value,
        flatb::GeometryUnion_box,
        box_fb.Union()
    );
}

}  // namespace _geom

namespace geom {
BoxPtr box() {
    auto box = std::make_shared<_geom::Box>();
    // _global::geometries.add(box->id, box);
    return box;
}

}  // namespace geom
}  // namespace slamd