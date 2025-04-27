#include <spdlog/spdlog.h>
#include <format>
#include <slamd_common/data/image.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <stdexcept>

namespace slamd {
namespace data {

Image::Image(
    const std::vector<uint8_t>& data,
    size_t width,
    size_t height,
    size_t channels
)
    : data(data),
      width(width),
      height(height),
      channels(channels) {
    if (data.size() != width * height * channels) {
        throw std::runtime_error(
            std::format(
                "Data should be {}, found {}",
                width * height * channels,
                data.size()
            )
        );
    }
}

Image::Image(
    std::vector<uint8_t>&& data,
    size_t width,
    size_t height,
    size_t channels
)
    : data(data),
      width(width),
      height(height),
      channels(channels) {
    if (data.size() != width * height * channels) {
        throw std::runtime_error(
            std::format(
                "Data should be {}, found {}",
                width * height * channels,
                data.size()
            )
        );
    }
}

flatbuffers::Offset<slamd::flatb::ImageData> Image::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto data_serialized = gmath::serialize_vector(builder, this->data);
    return flatb::CreateImageData(
        builder,
        this->width,
        this->height,
        data_serialized
    );
}

Image Image::deserialize(
    const flatb::ImageData* image_fb
) {
    auto deserialized_vector = gmath::deserialize_vector(image_fb->pixels());
    return Image(deserialized_vector, image_fb->width(), image_fb->height(), 3);
}

}  // namespace data
}  // namespace slamd