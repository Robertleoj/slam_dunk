#include <slamd/geometry/geometry.hpp>

namespace slamd {
namespace _geometry {

std::optional<gmath::AABB> Geometry::bounds() {
    return std::nullopt;
}

}  // namespace _geometry
}  // namespace slamd