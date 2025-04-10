#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <slamd/slamd.hpp>
#include <vector>

float random_float(
    float start,
    float end
) {
    float norm = static_cast<float>(rand()) / RAND_MAX;
    return norm * (end - start) + start;
}

float surface(
    float x,
    float y
) {
    return 3.0f * glm::cos(x) * glm::sin(y) + 0.1f * x * y + 0.2f * y;
}

std::shared_ptr<slamd::geometry::PointCloud> cool_point_cloud() {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;
    std::vector<float> radii;

    float min = -10.0f;
    float max = 10.0f;

    float density = 200.0f;
    float step = (max - min) / density;

    for (float i = min; i < max; i += step) {
        for (float j = min; j < max; j += step) {
            positions.emplace_back(i, j, surface(i, j));
        }
    }

    size_t count = positions.size();

    colors.reserve(count);
    radii.reserve(count);

    for (int i = 0; i < count; ++i) {
        positions.emplace_back(
            random_float(-100.f, 100.f),
            random_float(-100.f, 100.f),
            random_float(-100.f, 100.f)

        );

        colors.emplace_back(
            random_float(0.f, 1.f),
            random_float(0.f, 1.f),
            random_float(0.f, 1.f)
        );

        radii.push_back(random_float(0.01f, 0.1f));  // min 0.1, max 0.6
    }

    return slamd::geometry::point_cloud(positions, colors, radii);
}

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));

    slamd::Window window("point_cloud", 1000, 1000);

    auto scene = slamd::scene();

    window.add_scene("scene", scene);

    auto point_cloud = cool_point_cloud();

    scene->set_object("/point_cloud", point_cloud);

    window.wait_for_close();
}