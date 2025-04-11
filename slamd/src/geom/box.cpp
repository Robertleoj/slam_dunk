#include <ranges>
#include <slamd/geom/box.hpp>
#include <slamd/geom/utils.hpp>

namespace slamd {
namespace _geom {

// clang-format off
// 6 faces * 4 vertices = 24 unique verts
const std::vector<glm::vec3> box_corners = {{
    // Back face
    {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
    // Front face
    {-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f}, {0.5f, 0.5f,  0.5f}, {-0.5f, 0.5f,  0.5f},
    // Left face
    {-0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f,  0.5f},
    // Right face
    {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f,  0.5f}, {0.5f, 0.5f,  0.5f}, {0.5f, 0.5f, -0.5f},
    // Top face
    {-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, 0.5f,  0.5f}, {-0.5f, 0.5f,  0.5f},
    // Bottom face
    {-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
}};


const std::vector<glm::vec3> vertex_colors = {{
    // Back face - red
    {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
    // Front face - green
    {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
    // Left face - blue
    {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
    // Right face - yellow
    {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
    // Top face - magenta
    {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
    // Bottom face - cyan
    {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f},
}};

const std::vector<uint32_t> box_indices = {{
    // Back face (Z-)
    0, 2, 1, 0, 3, 2,
    // Front face (Z+)
    4, 5, 6, 4, 6, 7,
    // Left face (X-)
    8, 10, 9, 8, 11, 10,
    // Right face (X+)
    12, 13, 14, 12, 14, 15,
    // Top face (Y+)
    16, 18, 17, 16, 19, 18,
    // Bottom face (Y-)
    20, 21, 22, 20, 22, 23
}};

const std::vector<glm::vec3> vertex_normals = {{
    // Back face (-Z)
    {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
    // Front face (+Z)
    {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
    // Left face (-X)
    {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
    // Right face (+X)
    {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
    // Top face (+Y)
    {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
    // Bottom face (-Y)
    {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
}};

auto get_mesh_data() {


    data::ColoredMesh mesh;

    for (const auto&[vert, col, norm]: std::views::zip(box_corners, vertex_colors, vertex_normals)) {
        mesh.vertices.emplace_back(vert, col, norm);
    }

    mesh.triangle_indices = box_indices;

    return mesh;
}

// clang-format on

Box::Box()
    : box_mesh(get_mesh_data()) {}

void Box::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->box_mesh.render(model, view, projection);
}

}  // namespace _geom

namespace geom {
std::shared_ptr<Box> box() {
    return std::make_shared<Box>();
}

}  // namespace geom
}  // namespace slamd