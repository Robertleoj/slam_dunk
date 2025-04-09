#pragma once
#include <glm/glm.hpp>
#include <slamd/geometry/geometry.hpp>
#include <slamd/tree/node.hpp>
#include <slamd/tree_path.hpp>

namespace slamd {
namespace _tree {

template <typename D, typename T>
class Tree {
    using NodeT = Node<D, T>;

   private:
    std::unique_ptr<NodeT> root;

   public:
    Tree() { this->root = std::make_unique<NodeT>(); }

    void set_object(
        const TreePath& path,
        std::shared_ptr<D> object
    ) {
        if (path.is_root()) {
            throw std::runtime_error("Setting root object is not allowed");
        }

        NodeT* node = this->make_path(path);
        node->set_object(object);
    }

    void set_object_weak(
        const TreePath& path,
        std::weak_ptr<D> object
    ) {
        if (path.is_root()) {
            throw std::runtime_error("Setting root object is not allowed");
        }

        NodeT* node = this->make_path(path);
        node->set_object(object);
    }

    void set_transform(
        const TreePath& path,
        const T& transform
    ) {
        if (path.is_root()) {
            throw std::runtime_error("Setting root transform is not allowed");
        }

        NodeT* node = this->make_path(path);
        node->set_transform(transform);
    }

    void render(
        const T& view,
        const T& projection
    ) const {
        this->render_recursive(this->root.get(), T(1.0), view, projection);
    }

   private:
    void render_recursive(
        const NodeT* node,
        const T current_transform,
        const T& view,
        const T& projection
    ) const {
        T next_transform = current_transform;
        auto node_transform = node->get_transform();

        if (node_transform.has_value()) {
            next_transform = current_transform * node_transform.value();
        }

        const auto node_object = node->get_object();

        if (node_object.has_value()) {
            node_object.value()->render(next_transform, view, projection);
        }

        for (auto& [_, child] : node->children) {
            this->render_recursive(
                child.get(), next_transform, view, projection
            );
        }
    }

    NodeT* make_path(
        TreePath path
    ) {
        if (path.components.size() == 0) {
            return this->root.get();
        }

        NodeT* current_node = root.get();

        for (size_t i = 0; i < path.components.size(); i++) {
            auto& component = path.components[i];

            auto child_iterator = current_node->children.find(component);

            if (child_iterator == current_node->children.end()) {
                // in this case, we want to create the path down to the target
                // node
                std::unique_ptr<NodeT> new_node = std::make_unique<NodeT>();
                NodeT* new_node_ptr = new_node.get();

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
};

}  // namespace _tree

using SceneNode = _tree::Node<geometry::Geometry, glm::mat4>;
using Scene = _tree::Tree<geometry::Geometry, glm::mat4>;

std::shared_ptr<Scene> scene();

}  // namespace slamd