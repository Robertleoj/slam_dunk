#include <slamd/geom/circles_2d.hpp>
#include <slamd/global_object_map.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {

namespace _geom {

Circles2D::Circles2D(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
)
    : positions(positions),
      colors(colors),
      radii(radii),
      thickness(thickness) {}

flatbuffers::Offset<slamd::flatb::Geometry> Circles2D::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto pos_fb = gmath::serialize_vector(builder, this->positions);
    auto colors_fb = gmath::serialize_vector(builder, this->colors);
    auto radii_fb = builder.CreateVector(this->radii);

    auto circles_fb =
        flatb::CreateCircles2D(builder, pos_fb, colors_fb, radii_fb, thickness);

    return flatb::CreateGeometry(
        builder,
        flatb::GeometryUnion_circles_2d,
        circles_fb.Union()
    );
}

void Circles2D::update_positions(
    const std::vector<glm::vec2>& positions
) {}

void Circles2D::update_colors(
    const std::vector<glm::vec3>& colors
) {}

void Circles2D::update_radii(
    const std::vector<float>& radii
) {}

}  // namespace _geom

namespace geom2d {

CirclesPtr circles(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
) {
    auto circles =
        std::make_shared<_geom::Circles2D>(positions, colors, radii, thickness);
    // _global::geometries.add(circles->id, circles);
    return circles;
}
}  // namespace geom2d
}  // namespace slamd