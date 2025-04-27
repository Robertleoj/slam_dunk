#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <slamd/slamd.hpp>
#include <vector>

std::random_device rd;
std::mt19937 gen(rd());

float random_float(float start, float end) {
    std::uniform_real_distribution<float> dist(start, end);
    return dist(gen);
}

slamd::data::MeshData generate_trippy_ribbon_mesh() {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> normals;

    int segments = 200;
    float width = 1.0f;
    float radius = 5.0f;
    float twist = 5.0f; // number of twists

    for (int i = 0; i <= segments; ++i) {
        float t = (float)i / (float)segments;
        float angle = glm::two_pi<float>() * t * twist;

        glm::vec3 center = glm::vec3(
            radius * glm::cos(glm::two_pi<float>() * t),
            radius * glm::sin(glm::two_pi<float>() * t),
            5.0f * glm::sin(2.0f * glm::two_pi<float>() * t)
        );

        glm::vec3 tangent = glm::normalize(glm::vec3(
            -radius * glm::sin(glm::two_pi<float>() * t),
            radius * glm::cos(glm::two_pi<float>() * t),
            10.0f * glm::cos(2.0f * glm::two_pi<float>() * t)
        ));

        glm::vec3 normal = glm::normalize(glm::cross(tangent, glm::vec3(0, 0, 1)));
        glm::vec3 binormal = glm::normalize(glm::cross(tangent, normal));

        glm::vec3 offset1 = center + width * glm::cos(angle) * normal + width * glm::sin(angle) * binormal;
        glm::vec3 offset2 = center - width * glm::cos(angle) * normal - width * glm::sin(angle) * binormal;

        positions.push_back(offset1);
        positions.push_back(offset2);

        colors.emplace_back(random_float(0.4f, 1.0f), random_float(0.4f, 1.0f), random_float(0.4f, 1.0f));
        colors.emplace_back(random_float(0.4f, 1.0f), random_float(0.4f, 1.0f), random_float(0.4f, 1.0f));

        normals.push_back(glm::normalize(offset1 - center));
        normals.push_back(glm::normalize(offset2 - center));
    }

    for (int i = 0; i < segments; ++i) {
        int idx = i * 2;
        indices.push_back(idx);
        indices.push_back(idx + 1);
        indices.push_back(idx + 2);

        indices.push_back(idx + 1);
        indices.push_back(idx + 3);
        indices.push_back(idx + 2);
    }

    return slamd::data::MeshData(positions, colors, indices, normals);
}

int main() {
    slamd::Visualizer vis("trippy_ribbon");

    auto scene = slamd::scene();
    vis.add_scene("scene", scene);

    auto mesh_data = generate_trippy_ribbon_mesh();
    auto mesh = slamd::geom::mesh(mesh_data);

    scene->set_object("/trippy_ribbon", mesh);

    vis.hang_forever();
}
