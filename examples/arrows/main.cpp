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

slamd::geom::ArrowsPtr cool_arrows() {
    std::vector<glm::vec3> starts;
    std::vector<glm::vec3> ends;
    std::vector<glm::vec3> colors;

    int num_arrows = 500;

    for (int i = 0; i < num_arrows; ++i) {
        glm::vec3 start(
            random_float(-10.f, 10.f),
            random_float(-10.f, 10.f),
            random_float(-10.f, 10.f)
        );

        glm::vec3 direction(
            random_float(-1.f, 1.f),
            random_float(-1.f, 1.f),
            random_float(-1.f, 1.f)
        );

        glm::vec3 end = start + glm::normalize(direction) * random_float(0.5f, 2.0f);

        starts.push_back(start);
        ends.push_back(end);

        colors.emplace_back(
            random_float(0.f, 1.f),
            random_float(0.f, 1.f),
            random_float(0.f, 1.f)
        );
    }

    float thickness = 0.05f;
    return slamd::geom::arrows(starts, ends, colors, thickness);
}

int main() {
    slamd::Visualizer vis("arrows");

    auto scene = slamd::scene();

    vis.add_scene("scene", scene);

    auto arrows = cool_arrows();

    scene->set_object("/arrows", arrows);

    vis.hang_forever();
}
