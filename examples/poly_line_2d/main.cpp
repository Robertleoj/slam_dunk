#include <random>
#include <slamd/slamd.hpp>

std::random_device rd;
std::mt19937 gen(rd());

float random_float(
    float start,
    float end
) {
    std::uniform_real_distribution<float> dist(start, end);
    return dist(gen);
}

std::vector<glm::vec2> crazy_polyline_shape(
    float offset_x,
    float offset_y
) {
    std::vector<glm::vec2> points;

    int num_points = static_cast<int>(random_float(20, 80));
    float radius = random_float(100.f, 300.f);

    for (int i = 0; i < num_points; ++i) {
        float angle = i * (2.0f * glm::pi<float>() / num_points);
        float wiggle = random_float(0.8f, 1.2f);

        float x = offset_x + radius * wiggle * glm::cos(angle);
        float y = offset_y + radius * wiggle * glm::sin(angle);

        points.emplace_back(x, y);
    }

    return points;
}

slamd::geom2d::PolyLinePtr make_trippy_polyline() {
    float offset_x = random_float(200.f, 800.f);
    float offset_y = random_float(150.f, 500.f);

    auto points = crazy_polyline_shape(offset_x, offset_y);

    glm::vec3 color(
        random_float(0.3f, 1.f),
        random_float(0.3f, 1.f),
        random_float(0.3f, 1.f)
    );

    float thickness = random_float(2.f, 8.f);

    return slamd::geom2d::poly_line(points, color, thickness);
}

int main() {
    auto vis = slamd::visualizer("trippy_2d_polyline");

    auto canvas = slamd::canvas();
    vis->add_canvas("canvas", canvas);

    // Drop a whole bundle of random 2D poly lines
    for (int i = 0; i < 10; ++i) {
        auto polyline = make_trippy_polyline();
        canvas->set_object("/polyline_" + std::to_string(i), polyline);
    }

    vis->hang_forever();
}
