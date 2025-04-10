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
    std::vector<uint32_t> indices;

    for (const auto& [pos, col, rad] :
         std::views::zip(positions, colors, radii)) {
        size_t num_added = generate_sphere(vertices, indices, rad, 5, 5);

        vertex_colors.insert(vertex_colors.end(), num_added, col);
    }

    std::vector<data::ColoredVertex> colored_vertices;
    colored_vertices.reserve(vertices.size());
    for (const auto& [vert, col] : std::views::zip(vertices, vertex_colors)) {
        colored_vertices.emplace_back(vert, col);
    }

    return SimpleMesh(colored_vertices, indices);
}

}  // namespace _geometry
}  // namespace slamd