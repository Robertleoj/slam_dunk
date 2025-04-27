#include <slamd/geom/triad.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {
namespace _geom {

Triad::Triad(
    float scale,
    float thickness
) {}

}  // namespace _geom

namespace geom {

TriadPtr triad(
    float scale,
    float thickness
) {
    return std::make_shared<_geom::Triad>(scale, thickness);
}
}  // namespace geom

}  // namespace slamd