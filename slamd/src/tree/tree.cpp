#include <spdlog/spdlog.h>
#include <slamd/transforms.hpp>
#include <slamd/tree/tree.hpp>

namespace slamd {

void Scene::set_transform(
    const TreePath& path,
    glm::mat4 transform
) {
    this->set_transform_mat4(path, transform);
}

std::shared_ptr<Scene> scene() {
    return std::make_shared<Scene>();
}

void Canvas::set_transform(
    const TreePath& path,
    glm::mat3 transform
) {
    this->set_transform_mat4(path, xy_to_3d(transform));
}

std::shared_ptr<Canvas> canvas() {
    return std::make_shared<Canvas>();
}

}  // namespace slamd