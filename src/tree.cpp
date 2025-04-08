#include <spdlog/spdlog.h>
#include <slamd/tree.hpp>
#include <stack>

namespace slamd {

Node* Tree::make_path(
    TreePath path
) {
    if (path.components.size() == 0) {
        return this->tree_root.get();
    }

    Node* current_node = tree_root.get();

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

Tree::Tree() {
    this->tree_root = std::make_unique<Node>();
}

void insert_node() {}

void Tree::set_object(
    const TreePath& path,
    std::shared_ptr<geometry::Geometry> object
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    Node* node = this->make_path(path);
    node->set_object(object);
}

void Tree::set_object_weak(
    const TreePath& path,
    std::weak_ptr<geometry::Geometry> object
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    Node* node = this->make_path(path);
    node->set_object(object);
}

void Tree::set_transform(
    const TreePath& path,
    const glm::mat4& transform
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root transform is not allowed");
    }

    spdlog::debug("Setting transform for {}", path.string());

    Node* node = this->make_path(path);
    node->set_transform(transform);
}

void Tree::render(
    const glm::mat4 view,
    const glm::mat4 projection
) const {
    this->render_recursive(
        this->tree_root.get(), glm::mat4(1.0), view, projection
    );
}
void Tree::render_recursive(
    const Node* node,
    const glm::mat4 current_transform,
    const glm::mat4& view,
    const glm::mat4& projection
) const {
    glm::mat4 next_transform = current_transform;
    if (node->transform.has_value()) {
        next_transform = current_transform * node->transform.value();
    }

    const auto node_object = node->get_object();

    if (node_object.has_value()) {
        node_object.value()->render(next_transform, view, projection);
    }

    for (auto& [_, child] : node->children) {
        this->render_recursive(child.get(), next_transform, view, projection);
    }
}

}  // namespace slamd