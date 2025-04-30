#include <numbers>
#include <ranges>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_window/geom/points_2d.hpp>

namespace slamd {
namespace _geom {

std::shared_ptr<Points2D> Points2D::deserialize(
    const slamd::flatb::Points2D* points2d_fb
) {
    return std::make_shared<Points2D>(
        slamd::gmath::deserialize_vector(points2d_fb->positions()),
        slamd::gmath::deserialize_vector(points2d_fb->colors()),
        slamd::gmath::deserialize_vector(points2d_fb->radii())
    );
}

Mesh make_mesh(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
) {
    slamd::data::MeshData mesh_data;

    uint32_t curr_idx = 0;
    size_t num_segments = 10;
    float segment_size =
        std::numbers::pi * 2.0f / static_cast<float>(num_segments);

    for (const auto& [pos, col, rad] :
         std::views::zip(positions, colors, radii)) {
        // the center of the circle
        mesh_data
            .add_vertex(glm::vec3(pos, 0.0f), col, glm::vec3(0.0f, 0.0f, 1.0f));

        for (int i = 0; i < num_segments; i++) {
            float segment_start =
                std::numbers::pi * 2.0f * static_cast<float>(i) / num_segments;

            glm::vec3 start(
                glm::cos(segment_start),
                glm::sin(segment_start),
                0.0f
            );

            mesh_data.add_vertex(
                (start * rad) + glm::vec3(pos, 0.0f),
                col,
                glm::vec3(0.0f, 0.0f, 1.0f)
            );
        }

        for (uint32_t triangle_idx = 0; triangle_idx < num_segments;
             triangle_idx++) {
            uint32_t origin_idx = curr_idx;
            uint32_t start_idx = 1 + triangle_idx + curr_idx;
            uint32_t end_idx =
                1 + ((triangle_idx + 1) % num_segments) + curr_idx;
            mesh_data.triangle_indices.insert(
                mesh_data.triangle_indices.end(),
                {origin_idx, start_idx, end_idx}
            );
        }

        curr_idx += num_segments + 1;
    }

    return Mesh(mesh_data, 1.0f);
}

slamd::gmath::AABB make_bounds(
    const std::vector<glm::vec2>& positions,
    const std::vector<float>& radii
) {
    float min_x = positions[0].x - radii[0];
    float max_x = positions[0].x + radii[0];
    float min_y = positions[0].y - radii[0];
    float max_y = positions[0].y + radii[0];

    for (const auto& [pos, rad] : std::views::zip(positions, radii)) {
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

Points2D::Points2D(
    const std::vector<glm::vec2>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
)
    : mesh(make_mesh(positions, colors, radii)),
      cached_bounds(make_bounds(positions, radii)) {}

void Points2D::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->mesh.render(model, view, projection);
}

std::optional<slamd::gmath::AABB> Points2D::bounds() {
    return this->cached_bounds;
}

}  // namespace _geom
}  // namespace slamd