#include <slamd/geom/arrows.hpp>
#include <slamd_common/data/mesh.hpp>
#include <slamd_common/gmath/serialization.hpp>

namespace slamd {
namespace _geom {

constexpr uint32_t segments = 12;
struct ArrowMesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<uint32_t> indices;
};

Arrows::Arrows(
    const std::vector<glm::vec3>& starts,
    const std::vector<glm::vec3>& ends,
    const std::vector<glm::vec3>& colors,
    float thickness
)
    : starts(starts),
      ends(ends),
      colors(colors),
      thickness(thickness) {}

flatbuffers::Offset<slamd::flatb::Geometry> Arrows::serialize(
    flatbuffers::FlatBufferBuilder& builder
) {
    auto arrows_fb = flatb::CreateArrows(
        builder,
        gmath::serialize_vector(builder, this->starts),
        gmath::serialize_vector(builder, this->ends),
        gmath::serialize_vector(builder, this->colors),
        this->thickness
    );

    return flatb::CreateGeometry(
        builder,
        flatb::GeometryUnion_arrows,
        arrows_fb.Union()
    );
}

}  // namespace _geom

namespace geom {

ArrowsPtr arrows(
    const std::vector<glm::vec3>& starts,
    const std::vector<glm::vec3>& ends,
    const std::vector<glm::vec3>& colors,
    float thickness
) {
    return std::make_shared<_geom::Arrows>(starts, ends, colors, thickness);
}
}  // namespace geom
}  // namespace slamd