#include <slamd_window/geom/camera_frustum.hpp>
#include <slamd_window/geom/circles_2d.hpp>
#include <slamd_window/geom/geometry.hpp>
#include <slamd_window/geom/image.hpp>
#include <slamd_window/geom/point_cloud.hpp>
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
        case (slamd::flatb::GeometryUnion_circles_2d): {
            return Circles2D::deserialize(geom_fb->geometry_as_circles_2d());
        }
        case (slamd::flatb::GeometryUnion_camera_frustum): {
            return CameraFrustum::deserialize(
                geom_fb->geometry_as_camera_frustum()
            );
        }
        case (slamd::flatb::GeometryUnion_point_cloud): {
            return PointCloud::deserialize(geom_fb->geometry_as_point_cloud());
        }
        case (slamd::flatb::GeometryUnion_image): {
            return Image::deserialize(geom_fb->geometry_as_image());
        }
        default: {
            throw std::runtime_error("Unknown geometry type");
        }
    }
}

}  // namespace _geom
}  // namespace slamdw