#include <spdlog/spdlog.h>
#include <slamd/transforms.hpp>
#include <slamd/tree/tree.hpp>

namespace slamd {

namespace _tree {

Tree::Tree() {
    this->root = std::make_unique<Node>();
}

void Tree::set_object(
    const TreePath& path,
    std::shared_ptr<_geometry::Geometry> object
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    Node* node = this->make_path(path);
    node->set_object(object);
}

void Tree::render(
    const glm::mat4& view,
    const glm::mat4& projection
) const {
    this->render_recursive(this->root.get(), glm::mat4(1.0), view, projection);
}

void Tree::set_object_weak(
    const TreePath& path,
    std::weak_ptr<_geometry::Geometry> object
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    Node* node = this->make_path(path);
    node->set_object(object);
}

void Tree::set_transform_mat4(
    const TreePath& path,
    const glm::mat4& transform
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root transform is not allowed");
    }

    Node* node = this->make_path(path);
    node->set_transform(transform);
}

void Tree::render_recursive(
    const Node* node,
    const glm::mat4& current_transform,
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

Node* Tree::make_path(
    TreePath path
) {
    if (path.components.size() == 0) {
        return this->root.get();
    }

    Node* current_node = root.get();

    for (size_t i = 0; i < path.components.size(); i++) {
        auto& component = path.components[i];

        auto child_iterator = current_node->children.find(component);

        if (child_iterator == current_node->children.end()) {
            // in this case, we want to create the path down to the target
            // node
            std::unique_ptr<Node> new_node = std::make_unique<Node>();
            Node* new_node_ptr = new_node.get();

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

std::optional<_geom::AABB> Tree::bounds_recursive(
    const Node* node,
    const glm::mat4& prev_transform
) {
    glm::mat4 current_transform = prev_transform;
    auto node_transform = node->get_transform();

    if (node_transform.has_value()) {
        current_transform = prev_transform * node_transform.value();
    }

    const auto node_object = node->get_object();

    std::vector<_geom::AABB> bounds;

    if (node_object.has_value()) {
        auto object_bounds = node_object.value()->bounds();
        if (object_bounds.has_value()) {
            bounds.push_back(object_bounds.value().transform(current_transform)
            );
        }
    }

    for (auto& [_, child] : node->children) {
        // this->render_recursive(child.get(), next_transform, view,
        // projection);

        auto child_transform =
            this->bounds_recursive(child.get(), current_transform);

        if (child_transform.has_value()) {
            bounds.push_back(child_transform.value());
        }
    }

    return _geom::AABB::combine(bounds);
}

std::optional<_geom::AABB> Tree::bounds() {
    return this->bounds_recursive(this->root.get(), glm::mat4(1.0f));
}

}  // namespace _tree

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