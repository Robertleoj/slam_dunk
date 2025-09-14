#include <slamd_common/gmath/misc.hpp>
#include <slamd_common/gmath/serialization.hpp>
#include <slamd_common/gmath/transforms.hpp>
#include <slamd_window/geom/plane.hpp>

namespace slamd {
namespace _geom {

std::shared_ptr<Plane> Plane::deserialize(
    const slamd::flatb::Plane* plane_fb
) {
    return std::make_shared<Plane>(
        slamd::gmath::deserialize(plane_fb->normal()),
        slamd::gmath::deserialize(plane_fb->point()),
        slamd::gmath::deserialize(plane_fb->color()),
        plane_fb->radius(),
        plane_fb->alpha()
    );
}

std::unique_ptr<Mesh> make_plane_mesh(
    const glm::vec3& normal,
    const glm::vec3& point,
    const glm::vec3& color,
    float radius,
    float alpha
) {
    // first make a plane reference frame
    glm::vec3 z_axis = glm::normalize(normal);
    glm::vec3 y_axis = gmath::get_orthogonal_vector(z_axis);
    glm::vec3 x_axis = glm::cross(y_axis, z_axis);

    auto world_to_plane = gmath::make_frame(x_axis, y_axis, z_axis, point);

    std::vector<glm::vec3> verts;
    std::vector<uint32_t> indices;

    // center of the circle
    verts.push_back(point);

    int num_segments = 500;
    float num_segments_f = (float)num_segments;

    for (int i = 0; i < num_segments; i++) {
        float ipol = (float)i;

        float angle1 = ipol * glm::two_pi<float>() / num_segments_f;
        float angle2 = (ipol + 1.0f) * glm::two_pi<float>() / num_segments_f;

        glm::vec3 vert1_plane =
            glm::vec3(glm::cos(angle1), glm::sin(angle1), 0.0f) * radius;
        glm::vec3 vert1 = gmath::transform_point(world_to_plane, vert1_plane);

        glm::vec3 vert2_plane =
            glm::vec3(glm::cos(angle2), glm::sin(angle2), 0.0f) * radius;
        glm::vec3 vert2 = gmath::transform_point(world_to_plane, vert2_plane);

        uint32_t angle1_idx = verts.size();
        uint32_t angle2_idx = angle1_idx + 1;

        verts.insert(verts.end(), {vert1, vert2});
        indices.insert(indices.end(), {0, angle1_idx, angle2_idx});
    }

    auto data = slamd::data::MeshDataBuilder()
                    .set_positions(verts)
                    .set_colors(color)
                    .set_indices(indices)
                    .set_alpha(alpha)
                    .compute_normals()
                    .build();

    return std::make_unique<Mesh>(data, 1.0);
}

Plane::Plane(
    const glm::vec3& normal,
    const glm::vec3& point,
    const glm::vec3& color,
    float radius,
    float alpha
)
    : mesh(make_plane_mesh(normal, point, color, radius, alpha)) {}

void Plane::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {
    mesh->render(model, view, projection);
}

}  // namespace _geom
}  // namespace slamd