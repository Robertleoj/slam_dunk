#include <spdlog/spdlog.h>
#include <slamd/scene.hpp>

namespace slamd {

SceneNode* Scene::make_path(
    TreePath path
) {
    if (path.components.size() == 0) {
        return this->tree_root.get();
    }

    SceneNode* current_node = tree_root.get();

    for (size_t i = 0; i < path.components.size(); i++) {
        auto& component = path.components[i];

        auto child_iterator = current_node->children.find(component);

        if (child_iterator == current_node->children.end()) {
            // in this case, we want to create the path down to the target
            // node
            std::unique_ptr<SceneNode> new_node = std::make_unique<SceneNode>();
            SceneNode* new_node_ptr = new_node.get();

            // insert the new child
            current_node->children.emplace(component, std::move(new_node));

            current_node = new_node_ptr;

        } else {
            // node exists, just use it as the current node
            current_node = child_iterator->second.get();
        }
    }

    return current_node;
}

Scene::Scene() {
    this->tree_root = std::make_unique<SceneNode>();
}

void insert_node() {}

void Scene::set_object(
    const TreePath& path,
    std::shared_ptr<geometry::Geometry> object
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    SceneNode* node = this->make_path(path);
    node->set_object(object);
}

void Scene::set_object_weak(
    const TreePath& path,
    std::weak_ptr<geometry::Geometry> object
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    SceneNode* node = this->make_path(path);
    node->set_object(object);
}

void Scene::set_transform(
    const TreePath& path,
    const glm::mat4& transform
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root transform is not allowed");
    }

    SceneNode* node = this->make_path(path);
    node->set_transform(transform);
}

void Scene::render(
    const glm::mat4 view,
    const glm::mat4 projection
) const {
    this->render_recursive(
        this->tree_root.get(), glm::mat4(1.0), view, projection
    );
}
void Scene::render_recursive(
    const SceneNode* node,
    const glm::mat4 current_transform,
    const glm::mat4& view,
    const glm::mat4& projection
) const {
    glm::mat4 next_transform = current_transform;
    auto node_transform = node->get_transform();
    if (node_transform.has_value()) {
        next_transform = current_transform * node_transform.value();
    }

    const auto node_object = node->get_object();

    if (node_object.has_value()) {
        node_object.value()->render(next_transform, view, projection);
    }

    for (auto& [_, child] : node->children) {
        this->render_recursive(child.get(), next_transform, view, projection);
    }
}

std::shared_ptr<Scene> scene() {
    return std::make_shared<Scene>();
}

}  // namespace slamd