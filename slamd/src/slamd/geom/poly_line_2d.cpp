#include <slamd/geom/poly_line_2d.hpp>

#include <slamd_common/gmath/serialization.hpp>

namespace slamd {
namespace _geom {

PolyLine2D::PolyLine2D(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
)
    : points(points),
      color(color),
      thickness(thickness) {}

flatbuffers::Offset<slamd::flatb::Geometry> PolyLine2D::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto color_fb = gmath::serialize(this->color);
    auto poly_line2d_fb = flatb::CreatePolyLine2D(
        builder,
        gmath::serialize_vector(builder, this->points),
        &color_fb,
        this->thickness
    );

    return flatb::CreateGeometry(
        builder,
        this->id.value,
        flatb::GeometryUnion_poly_line_2d,
        poly_line2d_fb.Union()
    );
}

}  // namespace _geom

namespace geom2d {

PolyLinePtr poly_line(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
) {
    auto line = std::make_shared<_geom::PolyLine2D>(points, color, thickness);
    // _global::geometries.add(line->id, line);
    return line;
}

}  // namespace geom2d
}  // namespace slamd
