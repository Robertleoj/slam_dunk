#include <glm/glm.hpp>
#include <slamd/slamd.hpp>

int main() {
    auto vis = slamd::visualizer("two_scenes");

    auto scene1 = slamd::scene();
    auto scene2 = slamd::scene();

    vis->add_scene("scene 1", scene1);
    scene1->set_object("/box", slamd::geom::box());

    vis->add_scene("scene 2", scene2);
    scene2->set_object("/origin", slamd::geom::triad());
    scene2->set_object("/ball", slamd::geom::sphere(2.0f));

    glm::mat4 sphere_transform(1.0);
    sphere_transform[3] += glm::vec4(5.0, 1.0, 2.0, 1.0);
    scene2->set_transform("/ball", sphere_transform);

    vis->hang_forever();
}