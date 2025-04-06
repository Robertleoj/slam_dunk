#include <slam_dunk/tree.hpp>

namespace sdunk {

// std::optional<Tree::TraversalResult> Tree::traverse_to(
//     TreePath path,
//     bool create_path
// ) {

// }

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
    TreePath path,
    std::shared_ptr<SceneObject> object
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    Node* node = this->make_path(path);
    node->set_object(object);
}

void Tree::set_object(
    TreePath path,
    std::weak_ptr<SceneObject> object
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root object is not allowed");
    }

    Node* node = this->make_path(path);
    node->set_object(object);
}

void Tree::set_transform(
    TreePath path,
    glm::mat4 transform
) {
    if (path.is_root()) {
        throw std::runtime_error("Setting root transform is not allowed");
    }

    Node* node = this->make_path(path);
    node->set_transform(transform);
}

}  // namespace sdunk