#pragma once
#include <slamd/geom/geometry.hpp>
#include <slamd/geom/mesh.hpp>

namespace slamd {
namespace _geom {

class Box : public Geometry {
   public:
    Box();

    flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    ) override;
};

}  // namespace _geom

namespace geom {

using BoxPtr = std::shared_ptr<_geom::Box>;

BoxPtr box();
}  // namespace geom

}  // namespace slamd