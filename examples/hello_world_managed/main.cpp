#include <glm/glm.hpp>
#include <slamd/slamd.hpp>

int main() {
    slamd::Window window(1000, 1000);

    auto scene1 = slamd::scene();
    auto scene2 = slamd::scene();

    window.add_scene("scene 1", scene1);
    scene1->tree.set_object("/box", slamd::geometry::box());

    window.add_scene("scene 2", scene2);
    scene2->tree.set_object("/origin", slamd::geometry::triad());
    scene2->tree.set_object("/ball", slamd::geometry::sphere(2.0f));

    glm::mat4 sphere_transform(1.0);
    sphere_transform[3] += glm::vec4(5.0, 1.0, 2.0, 1.0);
    scene2->tree.set_transform("/ball", sphere_transform);

    window.wait_for_close();
}