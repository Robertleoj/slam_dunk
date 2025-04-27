#include <slamd/slamd.hpp>

int main() {
    slamd::Visualizer vis("hello_world");

    auto scene = slamd::scene();

    scene->set_object("/origin", slamd::geom::triad());

    vis.add_scene("scene", scene);

    vis.hang_forever();
}