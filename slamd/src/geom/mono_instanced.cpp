#include <spdlog/spdlog.h>
#include <format>
#include <slamd/geom/mono_instanced.hpp>

namespace slamd {
namespace _geom {

MonoInstanced::MonoInstanced(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& normals,
    const std::vector<uint32_t>& triangle_indices,
    const std::vector<glm::mat4>& transforms,
    const std::vector<glm::vec3>& colors
) {
    if (!(vertices.size() == normals.size())) {
        throw std::invalid_argument(
            std::format(
                "number of vertices {}, number of normals {}",
                vertices.size(),
                normals.size()
            )
        );
    }
    if (!((transforms.size() == colors.size()))) {
        throw std::invalid_argument(
            std::format(
                "number of transforms, and colors got "
                "{} transforms and {} colors",
                transforms.size(),
                colors.size()
            )
        );
    }
}

void MonoInstanced::update_transforms(
    const std::vector<glm::mat4>& transforms
) {}

void MonoInstanced::update_colors(
    const std::vector<glm::vec3>& colors
) {}

}  // namespace _geom
}  // namespace slamd