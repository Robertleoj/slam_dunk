#pragma once
#include <cstdint>
#include <vector>

namespace slamd {
namespace data {

class Image {
   public:
    Image(
        const std::vector<uint8_t>& data,
        size_t width,
        size_t height,
        size_t channels
    );

    Image (
        std::vector<uint8_t> &&data,
        size_t width,
        size_t height,
        size_t channels
    );

    const std::vector<uint8_t> data;
    const size_t width;
    const size_t height;
    const size_t channels;
};

}  // namespace data
}  // namespace slamd