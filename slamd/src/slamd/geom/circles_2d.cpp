#include <flatb/messages_generated.h>
#include <slamd/geom/circles_2d.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/transforms.hpp>
#include <slamd_common/utils/serialization.hpp>

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
        this->id.value,
        flatb::GeometryUnion_circles_2d,
        circles_fb.Union()
    );
}

void Circles2D::update_positions(
    const std::vector<glm::vec2>& positions
) {
    if (positions.size() != this->positions.size()) {
        throw std::invalid_argument(std::format(
            "Expected {} positions, got {}",
            this->positions.size(),
            positions.size()
        ));
    }

    this->positions = positions;

    // make the message
    flatbuffers::FlatBufferBuilder builder;

    auto positions_fb = gmath::serialize_vector(builder, positions);

    auto update_fb = flatb::CreateUpdateCircles2DPositions(
        builder,
        this->id.value,
        positions_fb
    );
    auto message_fb = flatb::CreateMessage(
        builder,
        flatb::MessageUnion_update_circles2d_positions,
        update_fb.Union()
    );

    builder.Finish(message_fb);

    this->broadcast(_utils::builder_buffer(builder));
};

void Circles2D::update_colors(
    const std::vector<glm::vec3>& colors
) {
    if (colors.size() != this->colors.size()) {
        throw std::invalid_argument(std::format(
            "Expected {} colors, got {}",
            this->colors.size(),
            colors.size()
        ));
    }

    this->colors = colors;

    // make the message
    flatbuffers::FlatBufferBuilder builder;

    auto colors_fb = gmath::serialize_vector(builder, colors);

    auto update_fb =
        flatb::CreateUpdateCircles2DColors(builder, this->id.value, colors_fb);
    auto message_fb = flatb::CreateMessage(
        builder,
        flatb::MessageUnion_update_circles2d_colors,
        update_fb.Union()
    );

    builder.Finish(message_fb);

    this->broadcast(_utils::builder_buffer(builder));
};

void Circles2D::update_radii(
    const std::vector<float>& radii
) {
    if (radii.size() != this->radii.size()) {
        throw std::invalid_argument(std::format(
            "Expected {} radii, got {}",
            this->radii.size(),
            radii.size()
        ));
    }

    this->radii = radii;

    // make the message
    flatbuffers::FlatBufferBuilder builder;

    auto radii_fb = gmath::serialize_vector(builder, radii);

    auto update_fb =
        flatb::CreateUpdateCircles2DRadii(builder, this->id.value, radii_fb);
    auto message_fb = flatb::CreateMessage(
        builder,
        flatb::MessageUnion_update_circles2d_radii,
        update_fb.Union()
    );

    builder.Finish(message_fb);

    this->broadcast(_utils::builder_buffer(builder));
}

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