#pragma once
#include <memory>
#include <slamd/data/image.hpp>
#include <slamd/geom/geometry.hpp>

namespace slamd {
namespace _geom {

class Image : public Geometry {
   public:
    Image(data::Image&& image, bool normalized = true);
};

}  // namespace _geom

namespace geom {
using ImagePtr = std::shared_ptr<_geom::Image>;

ImagePtr image(data::Image&& image);
}  // namespace geom

namespace geom2d {
using ImagePtr = std::shared_ptr<_geom::Image>;

ImagePtr image(data::Image&& image);

}  // namespace geom2d

}  // namespace slamd