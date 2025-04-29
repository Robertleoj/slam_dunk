#include <slamd/slamd.hpp>

int main() {
    auto vis = slamd::visualizer("one_scene_two_views");

    auto scene = slamd::scene();

    // add the scene twice - this will give
    // two views of the same scene
    vis->add_scene("scene view 1", scene);
    vis->add_scene("scene view 2", scene);

    // example object - triad at origin
    scene->set_object("/origin", slamd::geom::triad());

    vis->hang_forever();
}