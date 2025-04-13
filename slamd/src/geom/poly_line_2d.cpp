#include <slamd/geom/poly_line_2d.hpp>
#include <stdexcept>

namespace slamd {
namespace _geom {

Mesh PolyLine2D::make_mesh(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
) {
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;

    if (points.size() < 2) {
        throw std::invalid_argument(
            "cannot create poly line with less than 2 points"
        );
    }

    const float half_thickness = thickness * 0.5f;

    std::vector<glm::vec3> left_side, right_side;

    for (size_t i = 0; i < points.size(); ++i) {
        glm::vec2 dir;
        if (i == 0) {
            dir = glm::normalize(points[1] - points[0]);
        } else if (i == points.size() - 1) {
            dir = glm::normalize(points[i] - points[i - 1]);
        } else {
            glm::vec2 dir1 = glm::normalize(points[i] - points[i - 1]);
            glm::vec2 dir2 = glm::normalize(points[i + 1] - points[i]);
            dir = glm::normalize(dir1 + dir2);  // smooth corner
        }

        glm::vec2 normal = glm::vec2(-dir.y, dir.x);
        glm::vec3 offset = glm::vec3(normal * half_thickness, 0.0f);

        left_side.push_back(glm::vec3(points[i], 0.0f) - offset);
        right_side.push_back(glm::vec3(points[i], 0.0f) + offset);
    }

    for (size_t i = 0; i < points.size(); ++i) {
        vertices.push_back(left_side[i]);
        vertices.push_back(right_side[i]);
    }

    for (uint32_t i = 0; i < points.size() - 1; ++i) {
        uint32_t i0 = i * 2;
        uint32_t i1 = i * 2 + 1;
        uint32_t i2 = (i + 1) * 2;
        uint32_t i3 = (i + 1) * 2 + 1;

        // Triangle 1
        indices.push_back(i0);
        indices.push_back(i2);
        indices.push_back(i1);

        // Triangle 2
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    data::MeshData mesh_data = data::MeshDataBuilder()
                                   .set_positions(vertices)
                                   .set_colors(color)
                                   .set_normals(glm::vec3(0.0, 0.0, 1.0))
                                   .set_indices(indices)
                                   .build();

    return Mesh(mesh_data);
}

PolyLine2D::PolyLine2D(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
)
    : line_mesh(PolyLine2D::make_mesh(points, color, thickness)),
      cached_bounds(gmath::AABB::around_points(points)) {}

void PolyLine2D::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->line_mesh.render(model, view, projection);
}

std::optional<gmath::AABB> PolyLine2D::bounds() {
    return this->cached_bounds;
}

}  // namespace _geom

namespace geom2d {
using _geom::PolyLine2D;

std::shared_ptr<PolyLine2D> poly_line(
    const std::vector<glm::vec2>& points,
    const glm::vec3& color,
    float thickness
) {
    return std::make_shared<PolyLine2D>(points, color, thickness);
}

}  // namespace geom2d
}  // namespace slamd
