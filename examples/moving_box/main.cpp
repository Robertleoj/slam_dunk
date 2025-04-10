#include <chrono>
#include <glm/glm.hpp>
#include <slamd/gmath/angle.hpp>
#include <slamd/gmath/transforms.hpp>
#include <slamd/slamd.hpp>
#include <thread>

int main() {
    slamd::Window window("moving_box", 1000, 1000);

    auto scene = slamd::scene();

    scene->set_object("/rot/trans/box", slamd::geom::box());

    scene->set_transform("/rot/trans", slamd::gmath::tx3D(10.0f));

    glm::mat4 current_rot = glm::mat4(1.0);

    window.add_scene("scene", scene);

    while (true) {
        // rotate by 1 degree every 10ms
        current_rot =
            slamd::gmath::rz3D(slamd::gmath::Angle::deg(1.0)) * current_rot;

        scene->set_transform("/rot", current_rot);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}