#include <numbers>
#include <ranges>
#include <slamd/geom/circles_2d.hpp>
#include <slamd_common/gmath/transforms.hpp>

namespace slamd {

namespace _geom {

Circles2D::Circles2D(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
)
    : circles_instanced(
          Circles2D::make_mono_instanced(positions, colors, radii, thickness)
      ),
      colors(colors),
      positions(positions),
      radii(radii),
      cached_bounds(Circles2D::make_bounds(positions, radii)) {}

void Circles2D::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->handle_updates();
    this->circles_instanced.render(model, view, projection);
}

std::optional<gmath::AABB> Circles2D::bounds() {
    return this->cached_bounds;
}

MonoInstanced Circles2D::make_mono_instanced(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
) {
    size_t sections = 20;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    float small_rad = 1.0f - thickness;
    // float big_rad = 1.0f;

    float angle_step = 2.0f * std::numbers::pi / static_cast<float>(sections);

    for (size_t i = 0; i < sections; i++) {
        float angle = i * angle_step;

        glm::vec3 vc(glm::cos(angle), glm::sin(angle), 0.0f);

        vertices.insert(vertices.end(), {vc * small_rad, vc});
        normals.insert(normals.end(), 2, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    std::vector<uint32_t> indices;

    for (size_t i = 0; i < sections; i++) {
        uint32_t bottom_left = 2 * i;
        uint32_t top_left = 2 * i + 1;
        uint32_t bottom_right = (2 * (i + 1)) % vertices.size();
        uint32_t top_right = (2 * (i + 1) + 1) % vertices.size();

        // clang-format off
        indices.insert(
            indices.end(),
            {
                top_left, bottom_right, bottom_left,
                top_left, top_right, bottom_right
            }
        );
        //clang-format on

    }

    return MonoInstanced(
        vertices,
        normals,
        indices,
        Circles2D::make_transforms(
            positions,
            radii
        ),
        colors
    );

}

void Circles2D::update_positions(
    const std::vector<glm::vec2>& positions
) {
    this->positions = positions;
    this->pending_trans_update = true;
}

void Circles2D::update_colors(
    const std::vector<glm::vec3>& colors
) {
    this->colors = colors;
    this->pending_color_update = true;
}

void Circles2D::update_radii(
    const std::vector<float>& radii
) {
    this->radii = radii;
    this->pending_trans_update = true;
}

void Circles2D::handle_updates() {
    if (this->pending_trans_update) {
        this->circles_instanced.update_transforms(
            Circles2D::make_transforms(
                this->positions,
                this->radii
            )
        );
        this->pending_trans_update = false;
    }

    if (this->pending_color_update) {
        this->circles_instanced.update_colors(this->colors);
        this->pending_color_update = false;
    }
}

std::vector<glm::mat4> Circles2D::make_transforms(
    std::vector<glm::vec2> positions,
    std::vector<float> radii
) {
    std::vector<glm::mat4> transforms;

    transforms.reserve(positions.size());

    for(const auto &[pos, rad]: std::views::zip(positions, radii)) {
        transforms.push_back(
            gmath::t3D(glm::vec3(pos, 0.0f)) * gmath::scale_xy(glm::vec2(rad, rad))
        );
    }

    return transforms;
}

gmath::AABB Circles2D::make_bounds(
    const std::vector<glm::vec2>& positions,
    const std::vector<float>& radii
) {
    glm::vec2 pos_0 = positions[0];
    float rad_0 = radii[0];

    float min_x = pos_0.x - rad_0;

    float max_x = pos_0.x + rad_0;

    float min_y = pos_0.y - rad_0;
    float max_y = pos_0.y + rad_0;

    for (const auto& [pos, rad] : std::views::zip(positions, radii)) {
        min_x = std::fmin(pos.x - rad, min_x);
        max_x = std::fmax(pos.x + rad, max_x);

        min_y = std::fmin(pos.y - rad, min_y);
        max_y = std::fmax(pos.y + rad, max_y);
    }

    return gmath::AABB(
        glm::vec3(min_x, min_y, 0.0f),
        glm::vec3(max_x, max_y, 0.0f)
    );
}

}  // namespace _geom

namespace geom2d {

CirclesPtr circles(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
) {
    return std::make_shared<_geom::Circles2D>(
        positions,colors,radii,thickness
    );
}
}  // namespace geom2d
}  // namespace slamd