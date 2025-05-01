#include <chrono>
#include <cmath>
#include <glm/glm.hpp>
#include <slamd/slamd.hpp>
#include <thread>
#include <vector>

std::vector<glm::vec3> cool_spiral(
    int n,
    float t
) {
    std::vector<glm::vec3> points;
    float max_r = 10.0f;
    float t_start = t;
    float t_end = t + 20.0f * glm::pi<float>();

    for (int i = 0; i < n; ++i) {
        float alpha = static_cast<float>(i) / static_cast<float>(n - 1);
        float r = alpha * max_r;
        float angle = t_start + alpha * (t_end - t_start);

        float x = r * std::cos(angle);
        float y = r * std::sin(angle);
        float z = std::cos(x) * std::sin(y) + r;

        points.emplace_back(x, y, z);
    }

    return points;
}

int main() {
    auto vis = slamd::visualizer("3d_spiral");
    auto scene = slamd::scene();
    vis->add_scene("scene", scene);

    glm::vec3 pink(212.0f / 255.0f, 13.0f / 255.0f, 125.0f / 255.0f);

    float t = 0.0f;

    while (true) {
        auto coords = cool_spiral(1000, t);

        auto poly_line = slamd::geom::poly_line(coords, 0.7f, pink);

        scene->set_object("/poly_line", poly_line);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t += 0.05f;
    }
}
