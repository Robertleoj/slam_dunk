#include <spdlog/spdlog.h>
#include <chrono>
#include <glm/glm.hpp>
#include <slamd/slamd.hpp>
#include <thread>

int main() {
    spdlog::set_level(spdlog::level::debug);

    auto vis = slamd::visualizer("hello viz");

    auto scene = slamd::scene();

    vis->add_scene("scene1", scene);

    auto scene2 = slamd::scene();

    scene2->set_object("/origin/triad", slamd::geom::triad());

    glm::mat4 transform(1.0);
    transform[3][0] = 10.0f;

    scene2->set_transform("/origin", transform);

    vis->add_scene("scene2", scene2);

    auto canvas = slamd::canvas();

    vis->add_canvas("canvas", canvas);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}