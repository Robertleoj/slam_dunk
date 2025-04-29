#pragma once
#include <flatb/geometry_generated.h>
#include <glm/glm.hpp>
#include <slamd/id.hpp>
#include <slamd_common/gmath/aabb.hpp>

namespace slamd {

namespace _geom {

class Geometry {
   public:
    Geometry();
    virtual flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    virtual ~Geometry() = default;

   public:
    const _id::GeometryID id;
    std::set<_id::NodeID> attached_to;
};

}  // namespace _geom
}  // namespace slamd