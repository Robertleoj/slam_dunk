#pragma once
#include <cstdint>
#include <vector>

namespace slamdw {
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

    std::vector<uint8_t> data;
    size_t width;
    size_t height;
    size_t channels;
};

}  // namespace data
}  // namespace slamd