#include <slamd/slamd.hpp>

int main() {
    slamd::Window window(1000, 1000);

    auto scene = slamd::scene();

    window.add_scene("scene view 1", scene);
    window.add_scene("scene view 2", scene);

    scene->set_object("/box", slamd::geometry::triad());

    window.wait_for_close();
}