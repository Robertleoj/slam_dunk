#include <slamd_common/data/image.hpp>

namespace slamdw {
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
}  // namespace data
}  // namespace slamd