#include <format>
#include <glm/glm.hpp>
#include <random>
#include <slamd/slamd.hpp>

std::random_device rd;
std::mt19937 gen(rd());

float rand_float(
    float min,
    float max
) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

glm::vec3 random_vector(
    float min,
    float max
) {
    return glm::vec3(
        rand_float(min, max),
        rand_float(min, max),
        rand_float(min, max)
    );
}

glm::mat4 random_transform(
    bool scale = false
) {
    glm::vec3 position = random_vector(-10.0, 10.0);

    float angle = rand_float(0.0f, 359.0f);

    glm::vec3 axis = random_vector(0.0, 10.0);

    axis = glm::normalize(axis);  // make sure it's a unit vector

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

    if (scale) {
        glm::vec3 random_scale = random_vector(0.1f, 2.0f);

        transform = transform * glm::scale(glm::mat4(1.0f), random_scale);
    }

    return transform;
}

std::shared_ptr<slamd::geom::CameraFrustum> get_frustum() {
    float fx = 525.0f;
    float fy = 525.0f;
    float cx = 320.0f;
    float cy = 240.0f;

    glm::mat3 K(
        glm::vec3(fx, 0.0f, 0.0f),
        glm::vec3(0.0f, fy, 0.0f),
        glm::vec3(cx, cy, 1.0f)
    );

    size_t width = 640;
    size_t height = 480;

    return slamd::geom::camera_frustum(K, width, height, 1.0);
}

int main() {
    slamd::Window window("camera_frustum", 1000, 1000);

    auto scene = slamd::scene();

    auto frustum = get_frustum();

    for (size_t i = 0; i < 100; i++) {
        std::string key = std::format("/{}/frustum", i);

        scene->set_object(key, frustum);

        scene->set_transform(key, random_transform());
    }

    window.add_scene("scene", scene);

    window.wait_for_close();
}