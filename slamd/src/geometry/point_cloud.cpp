#include <ranges>
#include <slamd/geometry/point_cloud.hpp>
#include <slamd/geometry/utils.hpp>

namespace slamd {
namespace _geometry {

PointCloud::PointCloud(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
)
    : mesh(PointCloud::make_mesh(positions, colors, radii)) {}

void PointCloud::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->mesh.render(model, view, projection);
}

SimpleMesh PointCloud::make_mesh(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<float>& radii
) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> vertex_colors;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;

    for (const auto& [pos, col, rad] :
         std::views::zip(positions, colors, radii)) {
        size_t num_added =
            generate_sphere(vertices, indices, normals, rad, 10, 10);

        for (size_t i = 0; i < num_added; i++) {
            size_t idx = vertices.size() - i - 1;
            vertices[idx] += pos;
        }

        vertex_colors.insert(vertex_colors.end(), num_added, col);
    }

    data::ColoredMesh mesh_data;

    mesh_data.triangle_indices = std::move(indices);
    for (const auto& [vert, col, norm] :
         std::views::zip(vertices, vertex_colors, normals)) {
        mesh_data.vertices.emplace_back(vert, col, norm);
    }

    return SimpleMesh(mesh_data);
}

}  // namespace _geometry
}  // namespace slamd