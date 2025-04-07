#include <slam_dunk/scene_objects/poly_line.hpp>

namespace sdunk {

MonoMesh make_poly_line_mesh(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
) {
    std::vector<glm::vec3> verts;
    std::vector<uint32_t> indices;

    if (points.size() < 2) {
        throw std::invalid_argument("not enough points");
    }

    const glm::vec3 up = glm::vec3(0, 1, 0);  // simple billboard axis

    for (size_t i = 0; i < points.size() - 1; ++i) {
        glm::vec3 p0 = points[i];
        glm::vec3 p1 = points[i + 1];
        glm::vec3 dir = glm::normalize(p1 - p0);
        glm::vec3 right =
            glm::normalize(glm::cross(dir, up)) * (thickness * 0.5f);

        // Quad: 2 verts per point, 4 total
        glm::vec3 v0 = p0 - right;
        glm::vec3 v1 = p0 + right;
        glm::vec3 v2 = p1 + right;
        glm::vec3 v3 = p1 - right;

        uint32_t base = static_cast<uint32_t>(verts.size());
        verts.push_back(v0);
        verts.push_back(v1);
        verts.push_back(v2);
        verts.push_back(v3);

        // Two triangles per quad
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);

        indices.push_back(base + 0);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    }

    return MonoMesh(verts, indices, color);
}

PolyLine::PolyLine(
    const std::vector<glm::vec3>& points,
    float thickness,
    const glm::vec3& color
)
    : mesh(make_poly_line_mesh(points, thickness, color)) {}

void PolyLine::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    mesh.render(model, view, projection);
}

}  // namespace sdunk