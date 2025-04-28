#pragma once
#include <flatb/geometry_generated.h>
#include <atomic>
#include <glm/glm.hpp>
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

    const uint64_t id;

   private:
    static std::atomic<uint64_t> id_counter;
};

}  // namespace _geom
}  // namespace slamd