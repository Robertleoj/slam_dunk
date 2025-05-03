#include <slamd/slamd.hpp>

int main() {
    auto vis = slamd::visualizer("hello_world");

    auto scene = slamd::scene();

    scene->set_object("/origin", slamd::geom::triad());

    vis->add_scene("scene", scene);

    vis->hang_forever();
}