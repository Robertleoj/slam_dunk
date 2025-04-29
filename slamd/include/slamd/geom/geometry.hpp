#pragma once
#include <flatb/geometry_generated.h>
#include <glm/glm.hpp>
#include <slamd/id.hpp>
#include <slamd_common/gmath/aabb.hpp>

namespace slamd {

namespace _id {
struct GeometryTag {};
using GeometryID = ID<GeometryTag>;
}  // namespace _id
namespace _geom {

class Geometry {
   public:
    Geometry();
    virtual flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    virtual ~Geometry() = default;

    const _id::GeometryID id;
};

}  // namespace _geom
}  // namespace slamd