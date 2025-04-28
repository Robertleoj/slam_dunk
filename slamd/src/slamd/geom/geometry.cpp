#include <slamd/geom/geometry.hpp>
#include <slamd/global_object_map.hpp>

namespace slamd {
namespace _geom {

std::atomic<uint64_t> Geometry::id_counter = 1;

Geometry::Geometry()
    : id(Geometry::id_counter++) {}

flatbuffers::Offset<slamd::flatb::Geometry> Geometry::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    throw std::runtime_error("Serialization not implemented");
}

}  // namespace _geom
}  // namespace slamd