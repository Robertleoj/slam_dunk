#include <numbers>
#include <slamd/geom/poly_line.hpp>

namespace slamd {
namespace _geom {

MonoMesh make_poly_line_mesh(
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

    for (size_t i = 0; i < points.size(); ++i) {
        glm::vec3 forward;
        if (i == 0) {
            forward = glm::normalize(points[i + 1] - points[i]);
        } else if (i == points.size() - 1) {
            forward = glm::normalize(points[i] - points[i - 1]);
        } else {
            forward = glm::normalize(points[i + 1] - points[i - 1]);
        }

        glm::vec3 up = glm::vec3(0, 1, 0);
        if (glm::abs(glm::dot(forward, up)) > 0.9f) {
            up = glm::vec3(1, 0, 0);
        }

        glm::vec3 right = glm::normalize(glm::cross(forward, up));
        glm::vec3 local_up = glm::normalize(glm::cross(right, forward));

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

    return MonoMesh(verts, indices, color, min_brightness);
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
std::shared_ptr<PolyLine> poly_line(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
) {
    return std::make_shared<PolyLine>(points, thickness, color);
}
}  // namespace geom
}  // namespace slamd