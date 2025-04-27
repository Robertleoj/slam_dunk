#pragma once
#include <flatb/primitives_generated.h>
#include <cstdint>
#include <vector>

namespace slamd {
namespace data {

class Image {
   public:
    Image();
    Image(
        const std::vector<uint8_t>& data,
        size_t width,
        size_t height,
        size_t channels
    );

    Image(
        std::vector<uint8_t>&& data,
        size_t width,
        size_t height,
        size_t channels
    );

    flatbuffers::Offset<slamd::flatb::Image> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    static Image deserialize(const flatb::Image* image_fb);

    std::vector<uint8_t> data;
    size_t width;
    size_t height;
    size_t channels;
};

}  // namespace data
}  // namespace slamd