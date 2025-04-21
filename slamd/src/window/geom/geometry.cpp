#include <slamd_window/geom/geometry.hpp>

namespace slamdw {
namespace _geom {

std::optional<gmath::AABB> Geometry::bounds() {
    return std::nullopt;
}

}  // namespace _geom
}  // namespace slamd