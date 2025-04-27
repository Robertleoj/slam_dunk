#include <slamd_common/data/image.hpp>
#include <slamd_common/gmath/serialization.hpp>

namespace slamd {
namespace data {

Image::Image() {}
Image::Image(
    const std::vector<uint8_t>& data,
    size_t width,
    size_t height,
    size_t channels
)
    : data(data),
      width(width),
      height(height),
      channels(channels) {}

Image::Image(
    std::vector<uint8_t>&& data,
    size_t width,
    size_t height,
    size_t channels
)
    : data(data),
      width(width),
      height(height),
      channels(channels) {}

flatbuffers::Offset<slamd::flatb::Image> Image::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    return flatb::CreateImage(
        builder,
        this->width,
        this->height,
        gmath::serialize_vector(builder, this->data)
    );
}

Image Image::deserialize(
    const flatb::Image* image_fb
) {
    return Image(
        gmath::deserialize_vector(image_fb->data()),
        image_fb->width(),
        image_fb->height(),
        3
    );
}

}  // namespace data
}  // namespace slamd