#pragma once
#include <flatb/geometry_generated.h>
#include <glm/glm.hpp>
#include <map>
#include <slamd/tree/tree.hpp>
#include <slamd_common/gmath/aabb.hpp>
#include <slamd_common/id.hpp>

namespace slamd {

namespace _tree {
class Node;
}

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
    std::map<_id::NodeID, std::weak_ptr<_tree::Node>> attached_to;
};

}  // namespace _geom
}  // namespace slamd