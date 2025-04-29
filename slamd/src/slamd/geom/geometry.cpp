#include <slamd/geom/geometry.hpp>

namespace slamd {
namespace _geom {

Geometry::Geometry()
    : id(_id::GeometryID::next()) {}

flatbuffers::Offset<slamd::flatb::Geometry> Geometry::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    throw std::runtime_error("Serialization not implemented");
}

}  // namespace _geom
}  // namespace slamd