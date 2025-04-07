#include <slam_dunk/paths.hpp>
#include <slam_dunk/scene_objects/arcball_indicator.hpp>

namespace sdunk {

const fs::path vertex_shader_path =
    shader_folder() / "arcball_indicator" / "vertex_shader.vert";
const fs::path fragment_shader_path =
    shader_folder() / "arcball_indicator" / "fragment_shader.frag";

ArcballIndicator::ArcballIndicator()
    : shader(vertex_shader_path, fragment_shader_path) {}

void ArcballIndicator::render(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection
) {

};

void ArcballIndicator::set_arcball_zoom(
    float zoom
) {}

}  // namespace sdunk