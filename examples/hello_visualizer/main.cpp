#include <spdlog/spdlog.h>
#include <chrono>
#include <slamd/slamd.hpp>
#include <slamd/visualizer.hpp>
#include <thread>

int main() {
    spdlog::set_level(spdlog::level::debug);

    slamd::Visualizer vis("hello viz");

    auto scene = slamd::scene();

    vis.add_scene("scene1", scene);

    auto scene2 = slamd::scene();

    scene2->set_object("/origin/triad", slamd::geom::triad());

    vis.add_scene("scene2", scene2);

    auto canvas = slamd::canvas();

    vis.add_canvas("canvas", canvas);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}