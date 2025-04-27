#include <slamd/geom/image.hpp>

namespace slamd {
namespace _geom {

Image::Image(
    data::Image&& image,
    bool normalized
)
    : img(image),
      normalized(normalized) {}

flatbuffers::Offset<slamd::flatb::Geometry> Image::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto image_data_fb = this->img.serialize(builder);

    auto image_fb =
        flatb::CreateImage(builder, image_data_fb, this->normalized);

    return flatb::CreateGeometry(
        builder,
        flatb::GeometryUnion_image,
        image_fb.Union()
    );
}

}  // namespace _geom

namespace geom {

ImagePtr image(
    data::Image&& image
) {
    return std::make_shared<_geom::Image>(std::move(image), true);
}
}  // namespace geom

namespace geom2d {

ImagePtr image(
    data::Image&& image
) {
    return std::make_shared<_geom::Image>(std::move(image), false);
}

}  // namespace geom2d

}  // namespace slamd