#include <glm/glm.hpp>
#include <slamd/slamd.hpp>

int main() {
    slamd::Window window("circles", 1000, 1000);

    auto circles = slamd::geom2d::circles(
        {glm::vec2(0.0f, 0.0f)},
        {glm::vec3(1.0f, 0.0f, 0.0f)},
        {1.0f},
        0.1f
    );

    auto canvas = slamd::canvas();
    canvas->set_object("/circles", circles);

    window.add_canvas("canvas", canvas);

    window.wait_for_close();
}