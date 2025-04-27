#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <slamd/geom/arrows.hpp>
#include <slamd/geom/geometry.hpp>

namespace slamd {
namespace _geom {

class Triad : public Geometry {
   public:
    Triad(float scale, float thickness);
};

}  // namespace _geom

namespace geom {

using TriadPtr = std::shared_ptr<_geom::Triad>;

TriadPtr triad(float scale = 1.0, float thickness = 0.1);

}  // namespace geom

}  // namespace slamd