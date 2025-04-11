#include <slamd/geom/geometry.hpp>

namespace slamd {
namespace _geom {

std::optional<gmath::AABB> Geometry::bounds() {
    return std::nullopt;
}

}  // namespace _geom
}  // namespace slamd