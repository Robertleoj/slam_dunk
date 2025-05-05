#include <slamd/slamd.hpp>

int main() {
    auto vis = slamd::visualizer("hello_world");

    auto scene = vis->scene("scene");

    scene->set_object("/origin", slamd::geom::triad());

    vis->hang_forever();
}