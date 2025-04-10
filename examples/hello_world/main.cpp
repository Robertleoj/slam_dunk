#include <slamd/slamd.hpp>

int main() {
    slamd::Window window("hello_world", 1000, 1000);

    auto scene = slamd::scene();

    scene->set_object("/origin", slamd::geometry::triad());

    window.add_scene("scene", scene);

    window.wait_for_close();
}