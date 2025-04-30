#include <slamd_window/geom/geometry.hpp>

namespace slamd {
namespace _geom {

std::optional<slamd::gmath::AABB> Geometry::bounds() {
    return std::nullopt;
}

}  // namespace _geom
}  // namespace slamd