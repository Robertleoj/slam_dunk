#include <slamd/geom/image.hpp>

namespace slamd {
namespace _geom {

Image::Image(
    data::Image&& image,
    bool normalized
) {}

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