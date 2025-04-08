#include <slamd/scene_objects/arrows.hpp>

namespace slamd {

constexpr uint segments = 12;
struct ArrowMesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

ArrowMesh generate_cone(
    float radius,
    float height,
    glm::vec3 color
) {
    ArrowMesh mesh;
    glm::vec3 tip(0, 0, height);
    for (int i = 0; i <= segments; ++i) {
        float angle = glm::two_pi<float>() * i / segments;
        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;
        mesh.vertices.push_back({glm::vec3(x, y, 0), color});
    }
    mesh.vertices.push_back({tip, color});  // tip
    uint32_t tip_idx = mesh.vertices.size() - 1;
    for (int i = 0; i < segments; ++i) {
        mesh.indices.insert(
            mesh.indices.end(),
            {static_cast<uint32_t>(i), static_cast<uint32_t>(i + 1), tip_idx}
        );
    }
    return mesh;
}

void transform_geometry(
    std::vector<Vertex>& verts,
    const glm::mat4& xform
) {
    for (auto& v : verts) {
        v.position = glm::vec3(xform * glm::vec4(v.position, 1.0f));
    }
}

ArrowMesh generate_cylinder(
    float radius,
    float height,
    glm::vec3 color
) {
    ArrowMesh mesh;
    for (int i = 0; i <= segments; ++i) {
        float angle = glm::two_pi<float>() * i / segments;
        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;
        mesh.vertices.push_back({glm::vec3(x, y, 0), color});
        mesh.vertices.push_back({glm::vec3(x, y, height), color});
    }
    for (int i = 0; i < segments; ++i) {
        uint32_t a = i * 2;
        uint32_t b = a + 1;
        uint32_t c = a + 2;
        uint32_t d = a + 3;
        mesh.indices.insert(mesh.indices.end(), {a, b, d, a, d, c});
    }
    return mesh;
}

ArrowMesh generate_arrow(
    const glm::vec3& start,
    const glm::vec3& end,
    const glm::vec3& color,
    float thickness
) {
    ArrowMesh mesh;

    glm::vec3 dir = end - start;
    float length = glm::length(dir);
    if (length < 1e-4f) {
        return mesh;
    }

    glm::vec3 up =
        glm::abs(glm::dot(glm::normalize(dir), glm::vec3(0, 1, 0))) > 0.99f
            ? glm::vec3(1, 0, 0)
            : glm::vec3(0, 1, 0);

    glm::vec3 right = glm::normalize(glm::cross(dir, up));
    glm::vec3 forward = glm::normalize(glm::cross(right, dir));

    glm::mat3 basis = glm::mat3(right, forward, glm::normalize(dir));
    glm::mat4 transform = glm::mat4(basis);
    transform[3] = glm::vec4(start, 1.0f);

    float shaft_len = length * 0.8f;
    float head_len = length - shaft_len;

    auto shaft = generate_cylinder(thickness * 0.5f, shaft_len, color);
    transform_geometry(shaft.vertices, transform);
    mesh.vertices.insert(
        mesh.vertices.end(), shaft.vertices.begin(), shaft.vertices.end()
    );
    mesh.indices.insert(
        mesh.indices.end(), shaft.indices.begin(), shaft.indices.end()
    );

    glm::mat4 tip_xform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, shaft_len));
    tip_xform = transform * tip_xform;

    auto head = generate_cone(thickness * 0.75f, head_len, color);
    transform_geometry(head.vertices, tip_xform);
    uint32_t offset = mesh.vertices.size();
    for (auto& idx : head.indices) {
        idx += offset;
    }

    mesh.vertices.insert(
        mesh.vertices.end(), head.vertices.begin(), head.vertices.end()
    );
    mesh.indices.insert(
        mesh.indices.end(), head.indices.begin(), head.indices.end()
    );

    return mesh;
}

SimpleMesh make_arrows_mesh(
    const std::vector<glm::vec3>& starts,
    const std::vector<glm::vec3>& ends,
    const std::vector<glm::vec3>& colors,
    float thickness
) {
    std::vector<Vertex> verts;
    std::vector<uint32_t> inds;

    uint32_t index_offset = 0;
    for (size_t i = 0; i < starts.size(); ++i) {
        ArrowMesh mesh =
            generate_arrow(starts[i], ends[i], colors[i], thickness);

        verts.insert(verts.end(), mesh.vertices.begin(), mesh.vertices.end());

        for (auto idx : mesh.indices) {
            inds.push_back(idx + index_offset);
        }

        index_offset += mesh.vertices.size();
    }

    return SimpleMesh(verts, inds);
}

Arrows::Arrows(
    const std::vector<glm::vec3>& starts,
    const std::vector<glm::vec3>& ends,
    const std::vector<glm::vec3>& colors,
    float thickness
)
    : mesh(make_arrows_mesh(starts, ends, colors, thickness)) {}

void Arrows::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    mesh.render(model, view, projection);
}
}  // namespace slamd