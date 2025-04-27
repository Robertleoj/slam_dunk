#pragma once
#include <flatb/geometry_generated.h>
#include <glm/glm.hpp>
#include <optional>
#include <slamd_common/gmath/aabb.hpp>

namespace slamd {
namespace _geom {

class Geometry {
   public:
    virtual flatbuffers::Offset<slamd::flatb::Geometry> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    virtual ~Geometry() = default;
};

}  // namespace _geom
}  // namespace slamd