#include <slamd/scene_objects/box.hpp>

namespace slamd {

// clang-format off
// 6 faces * 4 vertices = 24 unique verts
constexpr std::array<glm::vec3, 24> box_corners = {{
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

constexpr std::array<glm::vec3, 24> vertex_colors = {{
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
    // Back face
    0, 1, 2, 2, 3, 0,
    // Front face
    4, 5, 6, 6, 7, 4,
    // Left face
    8, 9, 10, 10, 11, 8,
    // Right face
    12, 13, 14, 14, 15, 12,
    // Top face
    16, 17, 18, 18, 19, 16,
    // Bottom face
    20, 21, 22, 22, 23, 20
}};

// clang-format on

std::vector<Vertex> make_box_vertices() {
    std::vector<Vertex> vertices;
    vertices.reserve(8);

    for (int i = 0; i < 24; i++) {
        vertices.push_back({box_corners[i], vertex_colors[i]});
    }

    return vertices;
}

Box::Box()
    : box_mesh(make_box_vertices(), std::vector(box_indices)) {}

void Box::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    this->box_mesh.render(model, view, projection);
}

}  // namespace slamd