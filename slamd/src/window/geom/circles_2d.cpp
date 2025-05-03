#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/transforms.hpp>
#include <slamd_common/numbers.hpp>
#include <slamd_window/geom/circles_2d.hpp>

namespace slamd {

namespace _geom {

Circles2D::Circles2D(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii,
    float thickness
)
    : cached_bounds(Circles2D::make_bounds(positions, radii)),
      colors(colors),
      positions(positions),
      radii(radii) {
    this->circles_instanced =
        Circles2D::make_mono_instanced(positions, colors, radii, thickness);
}

std::shared_ptr<Circles2D> Circles2D::deserialize(
    const slamd::flatb::Circles2D* circles_fb
) {
    return std::make_shared<Circles2D>(
        slamd::gmath::deserialize_vector(circles_fb->positions()),
        slamd::gmath::deserialize_vector(circles_fb->colors()),
        slamd::gmath::deserialize_vector(circles_fb->radii()),
        circles_fb->thickness()
    );
}

void Circles2D::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->handle_updates();
    this->circles_instanced->render(model, view, projection);
}

std::optional<slamd::gmath::AABB> Circles2D::bounds() {
    return this->cached_bounds;
}

std::unique_ptr<MonoInstanced> Circles2D::make_mono_instanced(
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

    float angle_step = 2.0f * _num::pi / static_cast<float>(sections);

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

    return std::make_unique<MonoInstanced>(
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
        this->circles_instanced->update_transforms(
            Circles2D::make_transforms(
                this->positions,
                this->radii
            )
        );
        this->pending_trans_update = false;
    }

    if (this->pending_color_update) {
        this->circles_instanced->update_colors(this->colors);
        this->pending_color_update = false;
    }
}


std::vector<glm::mat4> Circles2D::make_transforms(
    std::vector<glm::vec2> positions,
    std::vector<float> radii
) {
    std::vector<glm::mat4> transforms;
    transforms.reserve(positions.size());

    for (std::size_t i = 0; i < positions.size(); ++i) {
        glm::vec2 pos = positions[i];
        float rad = radii[i];

        transforms.push_back(
            slamd::gmath::t3D(glm::vec3(pos, 0.0f)) * 
            slamd::gmath::scale_xy(glm::vec2(rad, rad))
        );
    }

    return transforms;
}

slamd::gmath::AABB Circles2D::make_bounds(
    const std::vector<glm::vec2>& positions,
    const std::vector<float>& radii
) {
    glm::vec2 pos_0 = positions[0];
    float rad_0 = radii[0];

    float min_x = pos_0.x - rad_0;
    float max_x = pos_0.x + rad_0;
    float min_y = pos_0.y - rad_0;
    float max_y = pos_0.y + rad_0;

    for (std::size_t i = 1; i < positions.size(); ++i) {
        glm::vec2 pos = positions[i];
        float rad = radii[i];

        min_x = std::fmin(pos.x - rad, min_x);
        max_x = std::fmax(pos.x + rad, max_x);
        min_y = std::fmin(pos.y - rad, min_y);
        max_y = std::fmax(pos.y + rad, max_y);
    }

    return slamd::gmath::AABB(
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