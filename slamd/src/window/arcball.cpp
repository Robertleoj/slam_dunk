#define GLM_ENABLE_EXPERIMENTAL

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <slamd_common/gmath/transforms.hpp>
#include <slamd_common/numbers.hpp>
#include <slamd_window/arcball.hpp>

namespace slamd {

Arcball::Arcball()
    : center(Arcball::default_center),
      theta(Arcball::default_theta),
      phi(Arcball::default_phi),
      radius(Arcball::default_radius) {}

void Arcball::rotate(
    slamd::gmath::Angle delta_theta,
    slamd::gmath::Angle delta_phi
) {
    // we rotate x angles around the z axis of
    this->theta = (this->theta + delta_theta);

    double vertical_range = (_num::pi / 2) - (_num::pi / 20);

    this->phi = (this->phi + delta_phi)
                    .clamp(
                        slamd::gmath::Angle::rad(-vertical_range),
                        slamd::gmath::Angle::rad(vertical_range)
                    );
}

void Arcball::translate_relative(
    glm::vec3 relative_amount
) {
    glm::vec3 amount = relative_amount * this->radius;
    glm::mat4 camera_in_center = this->camera_in_center();
    glm::mat4 center_in_camera = glm::inverse(camera_in_center);

    glm::mat4 camera_in_world = this->center * camera_in_center;

    glm::mat4 moved_camera = camera_in_world * slamd::gmath::t3D(amount);

    glm::mat4 new_center_in_world = moved_camera * center_in_camera;

    this->center = new_center_in_world;
}

void Arcball::zoom(
    float factor
) {
    this->radius = std::max(this->radius * factor, 0.1f);
}

glm::mat4 Arcball::camera_in_center() const {
    // start at x = 1
    glm::vec4 point_in_center_homo = slamd::gmath::rz3D(this->theta) *
                                     slamd::gmath::ry3D(this->phi) *
                                     glm::vec4(1.0, 0.0, 0.0, 1.0);

    glm::vec3 z_axis = glm::vec3(point_in_center_homo);

    glm::vec3 point_in_center = glm::vec3(point_in_center_homo) * this->radius;

    glm::vec3 down(0.0, 0.0, -1.0);

    glm::vec3 x_axis = glm::normalize(glm::cross(z_axis, down));
    glm::vec3 y_axis = glm::normalize(glm::cross(z_axis, x_axis));

    glm::mat4 center_to_cam = glm::mat4(1.0f);

    center_to_cam[0] = glm::vec4(x_axis, 0.0f);
    center_to_cam[1] = glm::vec4(y_axis, 0.0f);
    center_to_cam[2] = glm::vec4(z_axis, 0.0f);
    center_to_cam[3] = glm::vec4(point_in_center, 1.0);

    return center_to_cam;
}

glm::mat4 Arcball::view_matrix() const {
    return glm::inverse(this->center * this->camera_in_center());
}

void Arcball::reset() {
    this->center = Arcball::default_center;
    this->radius = Arcball::default_radius;
    this->theta = Arcball::default_theta;
    this->phi = Arcball::default_phi;
}

}  // namespace slamd