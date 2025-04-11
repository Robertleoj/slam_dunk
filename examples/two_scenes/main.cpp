#include <glm/glm.hpp>
#include <slamd/slamd.hpp>

int main() {
    slamd::Window window("two_scenes", 1000, 1000);

    auto scene1 = slamd::scene();
    auto scene2 = slamd::scene();

    window.add_scene("scene 1", scene1);
    scene1->set_object("/box", slamd::geom::box());

    window.add_scene("scene 2", scene2);
    scene2->set_object("/origin", slamd::geom::triad());
    scene2->set_object("/ball", slamd::geom::sphere(2.0f));

    glm::mat4 sphere_transform(1.0);
    sphere_transform[3] += glm::vec4(5.0, 1.0, 2.0, 1.0);
    scene2->set_transform("/ball", sphere_transform);

    window.wait_for_close();
}