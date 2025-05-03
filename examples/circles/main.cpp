#include <glm/glm.hpp>
#include <slamd/slamd.hpp>

int main() {
    auto vis = slamd::visualizer("circles");

    auto circles = slamd::geom2d::circles(
        {glm::vec2(0.0f, 0.0f)},
        {glm::vec3(1.0f, 0.0f, 0.0f)},
        {1.0f},
        0.1f
    );

    auto canvas = slamd::canvas();
    canvas->set_object("/circles", circles);

    vis->add_canvas("canvas", canvas);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}