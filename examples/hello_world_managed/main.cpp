#include <slamd/slamd.hpp>

int main() {
    slamd::Window window(1000, 1000);

    auto scene1 = slamd::scene();
    auto scene2 = slamd::scene();

    window.add_scene("scene 1", scene1);
    window.add_scene("scene 2", scene2);

    window.wait_for_close();
}