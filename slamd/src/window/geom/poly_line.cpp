#include <glm/gtc/matrix_transform.hpp>
#include <numbers>
#include <slamd_common/data/mesh.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_window/geom/poly_line.hpp>

namespace slamd {
namespace _geom {

std::shared_ptr<PolyLine> PolyLine::deserialize(
    const slamd::flatb::PolyLine* poly_line_fb
) {
    return std::make_shared<PolyLine>(
        slamd::gmath::deserialize_vector(poly_line_fb->points()),
        poly_line_fb->thickness(),
        slamd::gmath::deserialize(poly_line_fb->color()),
        poly_line_fb->min_brightness()
    );
}

Mesh make_poly_line_mesh(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color,
    float min_brightness
) {
    std::vector<glm::vec3> verts;
    std::vector<uint32_t> indices;

    if (points.size() < 2) {
        throw std::invalid_argument("not enough points");
    }

    const int segments = 8;
    const float radius = thickness * 0.5f;
    const float pi = std::numbers::pi;

    glm::vec3 prev_forward = glm::normalize(points[1] - points[0]);
    glm::vec3 prev_up = glm::vec3(0, 1, 0);
    if (glm::abs(glm::dot(prev_forward, prev_up)) > 0.9f) {
        prev_up = glm::vec3(1, 0, 0);
    }

    for (size_t i = 0; i < points.size(); i++) {
        glm::vec3 forward;
        if (i == points.size() - 1) {
            forward = glm::normalize(points[i] - points[i - 1]);
        } else {
            forward = glm::normalize(points[i + 1] - points[i]);
        }

        // Smooth frame transport
        glm::vec3 axis = glm::cross(prev_forward, forward);
        float axis_len = glm::length(axis);
        if (axis_len > 1e-6f) {
            float angle = glm::asin(axis_len);
            axis = glm::normalize(axis);
            glm::mat3 rot =
                glm::mat3(glm::rotate(glm::mat4(1.0f), angle, axis));
            prev_up = rot * prev_up;
        }

        glm::vec3 right = glm::normalize(glm::cross(forward, prev_up));
        glm::vec3 local_up = glm::normalize(glm::cross(right, forward));

        prev_forward = forward;

        uint32_t base = static_cast<uint32_t>(verts.size());

        for (int j = 0; j < segments; ++j) {
            float angle = (j / (float)segments) * 2 * pi;
            glm::vec3 offset =
                glm::cos(angle) * right + glm::sin(angle) * local_up;
            verts.push_back(points[i] + offset * radius);
        }

        if (i > 0) {
            for (int j = 0; j < segments; ++j) {
                int curr = j;
                int next = (j + 1) % segments;

                uint32_t a = base - segments + curr;
                uint32_t b = base - segments + next;
                uint32_t c = base + next;
                uint32_t d = base + curr;

                indices.push_back(a);
                indices.push_back(c);
                indices.push_back(b);

                indices.push_back(a);
                indices.push_back(d);
                indices.push_back(c);
            }
        }
    }

    auto data = slamd::data::MeshDataBuilder()
                    .set_positions(verts)
                    .set_colors(color)
                    .set_indices(indices)
                    .compute_normals()
                    .build();

    return Mesh(data);
}

PolyLine::PolyLine(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color,
    float min_brightness
)
    : mesh(make_poly_line_mesh(points, thickness, color, min_brightness)) {}

void PolyLine::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    mesh.render(model, view, projection);
}

}  // namespace _geom

namespace geom {

PolyLinePtr poly_line(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
) {
    return std::make_shared<_geom::PolyLine>(points, thickness, color);
}
}  // namespace geom
}  // namespace slamd