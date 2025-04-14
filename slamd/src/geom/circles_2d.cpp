#include <ranges>
#include <slamd/geom/circles_2d.hpp>

namespace slamd {

namespace _geom {

Circles2D::Circles2D(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
)
    : circles_instanced(
          Circles2D::make_mono_instanced(positions, colors, radii)
      ),
      positions(positions),
      radii(radii),
      cached_bounds(Circles2D::make_bounds(positions, radii)) {}

void Circles2D::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->circles_instanced.render(model, view, projection);
}

data::MeshData Circles2D::make_instance_mesh() {
    size_t sections = 15;

    data::MeshData mesh_data;

    for (size_t i = 0; i < sections; i++) {
    }
}

MonoInstanced Circles2D::make_mono_instanced(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
) {}

gmath::AABB Circles2D::make_bounds(
    const std::vector<glm::vec3>& positions,
    const std::vector<float>& radii
) {
    glm::vec2 pos_0 = positions[0];
    float rad_0 = radii[0];

    float min_x = pos_0.x - rad_0;
    float max_x = pos_0.x + rad_0;

    float min_y = pos_0.y - rad_0;
    float max_y = pos_0.y + rad_0;

    for (const auto& [pos, rad] : std::views::zip(positions, radii)) {
        min_x = pos.x - rad;
        max_x = pos.x + rad;

        min_y = pos.y - rad;
        max_y = pos.y + rad;
    }

    return gmath::AABB(
        glm::vec3(min_x, min_y, 0.0f),
        glm::vec3(max_x, max_y, 0.0f)
    );
}

}  // namespace _geom
}  // namespace slamd