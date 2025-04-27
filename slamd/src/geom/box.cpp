#include <ranges>
#include <slamd/geom/box.hpp>
#include <slamd_common/data/mesh.hpp>

namespace slamd {
namespace _geom {

Box::Box() {}

}  // namespace _geom

namespace geom {
BoxPtr box() {
    return std::make_shared<_geom::Box>();
}

}  // namespace geom
}  // namespace slamd