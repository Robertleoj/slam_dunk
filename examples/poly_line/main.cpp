#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <random>
#include <slamd/slamd.hpp>
#include <vector>

std::random_device rd;
std::mt19937 gen(rd());

float random_float(
    float start,
    float end
) {
    std::uniform_real_distribution<float> dist(start, end);
    return dist(gen);
}

slamd::geom::PolyLinePtr trippy_poly_line() {
    std::vector<glm::vec3> points;

    int num_points = static_cast<int>(random_float(50, 300));
    float freq_x = random_float(0.5f, 3.0f);
    float freq_y = random_float(0.5f, 3.0f);
    float freq_z = random_float(0.5f, 3.0f);

    float amp_x = random_float(2.0f, 8.0f);
    float amp_y = random_float(2.0f, 8.0f);
    float amp_z = random_float(2.0f, 8.0f);

    float phase_x = random_float(0.f, glm::pi<float>());
    float phase_y = random_float(0.f, glm::pi<float>());
    float phase_z = random_float(0.f, glm::pi<float>());

    for (int i = 0; i < num_points; ++i) {
        float t = i * 0.1f;

        float x = amp_x * glm::sin(freq_x * t + phase_x);
        float y = amp_y * glm::cos(freq_y * t + phase_y);
        float z = amp_z * glm::sin(freq_z * t + phase_z);

        points.emplace_back(x, y, z);
    }

    float thickness = random_float(0.02f, 0.15f);
    glm::vec3 color(
        random_float(0.2f, 1.f),
        random_float(0.2f, 1.f),
        random_float(0.2f, 1.f)
    );

    return slamd::geom::poly_line(points, thickness, color);
}

int main() {
    slamd::Visualizer vis("trippy_poly_lines");

    auto scene = slamd::scene();

    vis.add_scene("scene", scene);

    // Spawn a whole nebula of trippy lines
    int num_lines = 5;

    for (int i = 0; i < num_lines; ++i) {
        auto poly_line = trippy_poly_line();

        std::string name = "/poly_line_" + std::to_string(i);
        scene->set_object(name, poly_line);
    }

    vis.hang_forever();
}
