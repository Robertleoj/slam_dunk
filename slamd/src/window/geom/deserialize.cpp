#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/triad.hpp>

namespace slamdw {
namespace _geom {

std::shared_ptr<Geometry> Geometry::deserialize(
    const slamd::flatb::Geometry* geom_fb
) {
    switch (geom_fb->geometry_type()) {
        case (slamd::flatb::GeometryUnion_triad): {
            return Triad::deserialize(geom_fb->geometry_as_triad());
        }
        default: {
            throw std::runtime_error("Unknown geometry type");
        }
    }
}

}  // namespace _geom
}  // namespace slamdw