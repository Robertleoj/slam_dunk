#include <slamd/slamd.hpp>

int main() {
    slamd::Window window(1000, 1000);

    auto scene = slamd::scene();

    // add the scene twice - this will give
    // two views of the same scene
    window.add_scene("scene view 1", scene);
    window.add_scene("scene view 2", scene);

    // example object - triad at origin
    scene->set_object("/origin", slamd::geometry::triad());

    window.wait_for_close();
}